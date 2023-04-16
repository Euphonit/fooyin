/*
 * Fooyin
 * Copyright 2022-2023, Luke Taylor <LukeT1@proton.me>
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

#pragma once

#include "playlistitem.h"

#include <core/library/coverprovider.h>
#include <core/models/album.h>
#include <core/models/container.h>
#include <core/models/trackfwd.h>

#include <utils/treemodel.h>

#include <QAbstractItemModel>
#include <QPixmap>

namespace Fy {
namespace Utils {
class SettingsManager;
}

namespace Core {
namespace Player {
class PlayerManager;
}

namespace Playlist {
class PlaylistHandler;
}
} // namespace Core

namespace Gui::Widgets {
namespace Playlist {
enum Role
{
    Type = Qt::UserRole + 20,
    Mode = Qt::UserRole + 21,
};
}

class PlaylistModel : public Utils::TreeModel<PlaylistItem>
{
    Q_OBJECT

public:
    explicit PlaylistModel(Core::Player::PlayerManager* playerManager, Core::Playlist::PlaylistHandler* playlistHandler,
                           Utils::SettingsManager* settings, QObject* parent = nullptr);

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] QModelIndex matchTrack(int id) const;

    void reset();
    void setupModelData();
    void changeTrackState();

    [[nodiscard]] QModelIndex indexForTrack(const Core::Track& track) const;
    [[nodiscard]] QModelIndex indexForItem(PlaylistItem* item) const;

private:
    using PlaylistItemHash = std::unordered_map<QString, std::unique_ptr<PlaylistItem>>;

    void createAlbums(const Core::TrackList& tracks);
    PlaylistItem* iterateTrack(const Core::Track& track, bool discHeaders, bool splitDiscs);

    PlaylistItem* checkInsertKey(const QString& key, PlaylistItem::Type type, const ItemType& item,
                                 PlaylistItem* parent);

    void insertRow(PlaylistItem* parent, PlaylistItem* child);

    void beginReset();

    [[nodiscard]] QVariant trackData(PlaylistItem* item, int role) const;
    [[nodiscard]] QVariant albumData(PlaylistItem* item, int role) const;
    [[nodiscard]] QVariant containerData(PlaylistItem* item, int role) const;

    Core::Player::PlayerManager* m_playerManager;
    Core::Playlist::PlaylistHandler* m_playlistHandler;
    Utils::SettingsManager* m_settings;
    Core::Library::CoverProvider m_coverProvider;

    bool m_discHeaders;
    bool m_splitDiscs;
    bool m_altColours;
    bool m_simplePlaylist;

    bool m_resetting;

    PlaylistItemHash m_nodes;
    Core::AlbumHash m_albums;
    Core::ContainerHash m_containers;
    QPixmap m_playingIcon;
    QPixmap m_pausedIcon;
};
} // namespace Gui::Widgets
} // namespace Fy
