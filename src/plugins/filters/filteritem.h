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

#include <core/models/trackfwd.h>

#include <QObject>

namespace Fy::Filters {
enum FilterItemRole
{
    Title   = Qt::UserRole + 1,
    Tracks  = Qt::UserRole + 2,
    Sorting = Qt::UserRole + 3,
};

class FilterItem;
using ItemChildren = std::vector<FilterItem*>;

class FilterItem
{
public:
    FilterItem() = default;
    explicit FilterItem(QString title, QString sortTitle = "", bool isAllNode = false);

    [[nodiscard]] const ItemChildren& children() const;
    [[nodiscard]] FilterItem* child(int index) const;
    void appendChild(FilterItem* child);
    [[nodiscard]] int childCount() const;

    [[nodiscard]] QVariant data(int role) const;
    [[nodiscard]] int trackCount() const;
    void addTrack(const Core::Track& track);

    [[nodiscard]] bool isAllNode() const;
    [[nodiscard]] bool hasSortTitle() const;

    void sortChildren(Qt::SortOrder order);

private:
    QString m_title;
    QString m_sortTitle;
    Core::TrackList m_tracks;
    bool m_isAllNode;
    ItemChildren m_children;
};
} // namespace Fy::Filters
