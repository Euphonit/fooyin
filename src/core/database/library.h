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

#include "core/library/libraryinfo.h"
#include "module.h"

namespace Fy::Core::DB {
class Library : private Module
{
public:
    explicit Library(const QString& connectionName);

    Core::Library::LibraryList getAllLibraries();
    int insertLibrary(const QString& path, const QString& name);
    bool removeLibrary(int id);
    bool renameLibrary(int id, const QString& name);
};
} // namespace Fy::Core::DB
