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

#include "version.h"

#include <QApplication>
#include <pluginsystem/pluginmanager.h>

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(fonts);

    qApp->setApplicationName("fooyin");
    qApp->setApplicationVersion(VERSION);

    auto* app = new QApplication(argc, argv);
    auto* pluginManager = PluginSystem::PluginManager::instance();

    // TODO: Pass down CMake vars
    QString pluginsPath = QCoreApplication::applicationDirPath() + "/../lib/fooyin/plugins";
    pluginManager->findPlugins(pluginsPath);
    pluginManager->addPlugins();

    int result = QApplication::exec();

    delete app;

    return result;
}
