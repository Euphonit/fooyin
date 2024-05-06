/*
 * Fooyin
 * Copyright © 2023, Luke Taylor <LukeT1@proton.me>
 *
 * Fooyin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fooyin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fooyin.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <gui/coverprovider.h>

#include "core/tagging/tagreader.h"
#include "internalguisettings.h"

#include <core/scripting/scriptparser.h>
#include <core/track.h>
#include <gui/guiconstants.h>
#include <gui/guipaths.h>
#include <utils/async.h>
#include <utils/crypto.h>
#include <utils/settings/settingsmanager.h>
#include <utils/utils.h>

#include <QByteArray>
#include <QFileInfo>
#include <QPixmapCache>

#include <set>

constexpr QSize MaxCoverSize = {800, 800};

namespace {
QString generateCoverKey(const QString& hash, Fooyin::Track::Cover type, const QSize& size)
{
    return Fooyin::Utils::generateHash(QString::number(static_cast<int>(type)), hash, QString::number(size.width()),
                                       QString::number(size.height()));
}

QString coverThumbnailPath(const QString& key)
{
    return Fooyin::Gui::coverPath() + key + QStringLiteral(".jpg");
}

void saveThumbnail(const QImage& cover, const QString& key)
{
    QFile file{coverThumbnailPath(key)};
    file.open(QIODevice::WriteOnly);
    cover.save(&file, "JPG", 85);
}

QImage loadCover(const QString& path, const QSize& size)
{
    QImage image;
    image.load(path);
    image = Fooyin::Utils::scaleImage(image, size);
    return image;
}

QImage loadCover(const QByteArray& data, QSize size)
{
    QImage image;
    image.loadFromData(data);
    image = Fooyin::Utils::scaleImage(image, size);
    return image;
}

QPixmap loadCachedCover(const QString& key)
{
    QPixmap cover;

    if(QPixmapCache::find(key, &cover)) {
        return cover;
    }

    return {};
}
} // namespace

namespace Fooyin {
struct CoverProvider::Private
{
    CoverProvider* self;

    SettingsManager* settings;

    bool usePlacerholder{true};
    QString coverKey;
    std::set<QString> pendingCovers;
    ScriptParser parser;

    CoverPaths paths;

    std::mutex fetchGuard;

    explicit Private(CoverProvider* self_, SettingsManager* settings_)
        : self{self_}
        , settings{settings_}
        , paths{settings->value<Settings::Gui::Internal::TrackCoverPaths>().value<CoverPaths>()}
    {
        settings->subscribe<Settings::Gui::Internal::TrackCoverPaths>(
            self, [this](const QVariant& var) { paths = var.value<CoverPaths>(); });
    }

    QString findCover(const QString& key, const Track& track, Track::Cover type)
    {
        if(!track.isValid()) {
            return {};
        }

        QString cachePath = coverThumbnailPath(key);
        if(QFileInfo::exists(cachePath)) {
            return cachePath;
        }

        QStringList filters;

        const std::scoped_lock lock{fetchGuard};

        if(type == Track::Cover::Front) {
            for(const auto& path : paths.frontCoverPaths) {
                filters.emplace_back(parser.evaluate(path, track));
            }
        }
        else if(type == Track::Cover::Back) {
            for(const auto& path : paths.backCoverPaths) {
                filters.emplace_back(parser.evaluate(path, track));
            }
        }
        else if(type == Track::Cover::Artist) {
            for(const auto& path : paths.artistPaths) {
                filters.emplace_back(parser.evaluate(path, track));
            }
        }

        for(const auto& filter : filters) {
            const QFileInfo fileInfo{QDir::cleanPath(filter)};
            const QDir filePath{fileInfo.path()};
            const QString filePattern  = fileInfo.fileName();
            const QStringList fileList = filePath.entryList({filePattern}, QDir::Files);

            if(!fileList.isEmpty()) {
                return filePath.absolutePath() + QStringLiteral("/") + fileList.constFirst();
            }
        }

        return {};
    }

    void fetchCover(const QString& key, const Track& track, const QSize& size, Track::Cover type, bool saveToDisk)
    {
        Utils::asyncExec([this, key, track, size, type, saveToDisk]() {
            QImage image;

            const QString coverPath = findCover(key, track, type);
            if(!coverPath.isEmpty()) {
                // Prefer artwork in directory
                image = loadCover(coverPath, size);
            }

            if(image.isNull()) {
                const QByteArray coverData = Tagging::readCover(track, type);
                if(!coverData.isEmpty()) {
                    image = loadCover(coverData, size);
                }
            }

            if(saveToDisk) {
                const QString cachePath = coverThumbnailPath(key);
                if(image.isNull()) {
                    QFile::remove(cachePath);
                }
                else if(!QFileInfo::exists(cachePath)) {
                    saveThumbnail(image, key);
                }
            }

            return image;
        }).then(self, [this, key, track](const QImage& image) {
            if(image.isNull()) {
                return;
            }

            const QPixmap cover = QPixmap::fromImage(image);
            if(!QPixmapCache::insert(key, cover)) {
                qDebug() << "Failed to cache cover for:" << track.filepath();
            }

            pendingCovers.erase(key);
            emit self->coverAdded(track);
        });
    }

    QPixmap loadNoCover(const QSize& size, Track::Cover type) const
    {
        const QString key = generateCoverKey(QStringLiteral("|NoCover|"), type, size);

        QPixmap cachedCover;
        if(QPixmapCache::find(key, &cachedCover)) {
            return cachedCover;
        }

        const static QString noCoverKey = QString::fromLatin1(Constants::NoCover);

        QImage image;
        image.load(noCoverKey);
        image = Utils::scaleImage(image, size);

        if(image.isNull()) {
            return {};
        }

        const QPixmap cover = QPixmap::fromImage(image);
        if(!QPixmapCache::insert(key, cover)) {
            qDebug() << "Failed to cache placeholder cover";
        }

        return cover;
    }
};

CoverProvider::CoverProvider(SettingsManager* settings, QObject* parent)
    : QObject{parent}
    , p{std::make_unique<Private>(this, settings)}
{ }

void CoverProvider::setUsePlaceholder(bool enabled)
{
    p->usePlacerholder = enabled;
}

void CoverProvider::setCoverKey(const QString& name)
{
    p->coverKey = name;
}

void CoverProvider::resetCoverKey()
{
    p->coverKey.clear();
}

CoverProvider::~CoverProvider() = default;

QPixmap CoverProvider::trackCover(const Track& track, const QSize& size, Track::Cover type, bool saveToDisk) const
{
    if(!track.isValid()) {
        return p->usePlacerholder ? p->loadNoCover(size, type) : QPixmap{};
    }

    const QString coverKey = !p->coverKey.isEmpty() ? p->coverKey : generateCoverKey(track.albumHash(), type, size);

    if(!p->pendingCovers.contains(coverKey)) {
        QPixmap cover = loadCachedCover(coverKey);
        if(!cover.isNull()) {
            return cover;
        }

        p->pendingCovers.emplace(coverKey);
        p->fetchCover(coverKey, track, size, type, saveToDisk);
    }

    return p->usePlacerholder ? p->loadNoCover(size, type) : QPixmap{};
}

QPixmap CoverProvider::trackCover(const Track& track, Track::Cover type, bool saveToDisk) const
{
    return trackCover(track, MaxCoverSize, type, saveToDisk);
}

void CoverProvider::clearCache()
{
    QDir cache{Fooyin::Gui::coverPath()};
    cache.removeRecursively();

    QPixmapCache::clear();
}

void CoverProvider::removeFromCache(const QString& key)
{
    QDir cache{Fooyin::Gui::coverPath()};
    cache.remove(coverThumbnailPath(key));

    QPixmapCache::remove(key);
}
} // namespace Fooyin

#include "gui/moc_coverprovider.cpp"
