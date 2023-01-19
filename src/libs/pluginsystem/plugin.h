/*
 * Fooyin
 * Copyright 2022, Luke Taylor <LukeT1@proton.me>
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

#include "pluginsystem_global.h"

#include <QtPlugin>

namespace PluginSystem {
/*!
    The Plugin class is an abstract base class that each plugin must implement.

    An additional metadata json must also be supplied detailing plugin info
    including dependencies.

    Plugins must set their IID to "com.fooyin.plugin".
*/
class PLUGINSYSTEM_EXPORT Plugin
{
public:
    virtual ~Plugin() = default;

    virtual void initialise() {};
    virtual void finalise() {};
    virtual void pluginsFinalised() {};
    virtual void shutdown() {};
};
} // namespace PluginSystem

Q_DECLARE_INTERFACE(PluginSystem::Plugin, "com.fooyin.plugin")
