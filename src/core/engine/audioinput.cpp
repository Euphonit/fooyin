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

#include <core/engine/audioinput.h>

namespace Fooyin {
class AudioInputPrivate
{
public:
    int maxLoops{-1};
};

AudioInput::AudioInput()
    : p{std::make_unique<AudioInputPrivate>()}
{ }

AudioInput::~AudioInput() = default;

QByteArray AudioInput::readCover(const Track& /*track*/, Track::Cover /*cover*/)
{
    return {};
}

bool AudioInput::writeMetaData(const Track& /*track*/, const WriteOptions& /*options*/)
{
    return false;
}

int AudioInput::maxLoops() const
{
    return p->maxLoops;
}

void AudioInput::setMaxLoops(int count)
{
    p->maxLoops = count;
}
} // namespace Fooyin
