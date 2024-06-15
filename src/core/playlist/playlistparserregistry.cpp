/*
 * Fooyin
 * Copyright © 2024, Luke Taylor <LukeT1@proton.me>
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

#include "playlistparserregistry.h"

#include <core/playlist/playlistparser.h>

namespace Fooyin {
struct PlaylistParserRegistry::Private
{
    std::unordered_map<QString, std::unique_ptr<PlaylistParser>> m_parsers;
};

PlaylistParserRegistry::PlaylistParserRegistry()
    : p{std::make_unique<Private>()}
{ }

PlaylistParserRegistry::~PlaylistParserRegistry() = default;

PlaylistParser* PlaylistParserRegistry::registerParser(std::unique_ptr<PlaylistParser> parser)
{
    const QString name = parser->name();
    if(p->m_parsers.contains(name)) {
        return p->m_parsers.at(name).get();
    }

    return p->m_parsers.emplace(name, std::move(parser)).first->second.get();
}

QStringList PlaylistParserRegistry::supportedExtensions() const
{
    QStringList extensions;

    for(const auto& [_, parser] : p->m_parsers) {
        extensions.append(parser->supportedExtensions());
    }

    return extensions;
}

PlaylistParser* PlaylistParserRegistry::parserForExtension(const QString& extension) const
{
    for(const auto& [_, parser] : p->m_parsers) {
        if(parser->supportedExtensions().contains(extension)) {
            return parser.get();
        }
    }

    return nullptr;
}
} // namespace Fooyin
