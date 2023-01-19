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

#include "filtersettings.h"

#include <pluginsystem/pluginmanager.h>

namespace Filters::Settings {
FiltersSettings::FiltersSettings()
    : m_settings(PluginSystem::object<Core::SettingsManager>())
{
    m_settings->createSetting(Settings::FilterAltColours, false, "Filters");
    m_settings->createSetting(Settings::FilterHeader, true, "Filters");
    m_settings->createSetting(Settings::FilterScrollBar, true, "Filters");
}

FiltersSettings::~FiltersSettings() = default;
} // namespace Filters::Settings
