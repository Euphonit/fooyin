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

#pragma once

#include <QColor>
#include <QFont>
#include <QVariant>

namespace Fooyin {
struct RichFormatting
{
    QFont font;
    QColor colour;

    bool operator==(const RichFormatting& other) const
    {
        return std::tie(font, colour) == std::tie(other.font, other.colour);
    };
};

struct RichTextBlock
{
    QString text;
    RichFormatting format;

    bool operator==(const RichTextBlock& other) const
    {
        return std::tie(text, format) == std::tie(other.text, other.format);
    };
};

struct RichText
{
    std::vector<RichTextBlock> blocks;

    bool operator==(const RichText& other) const
    {
        return blocks == other.blocks;
    };

    operator QVariant() const
    {
        return QVariant::fromValue(*this);
    }

    [[nodiscard]] size_t size() const
    {
        return blocks.size();
    }

    [[nodiscard]] bool empty() const
    {
        return blocks.empty();
    }

    void clear()
    {
        blocks.clear();
    }

    [[nodiscard]] QString joinedText() const
    {
        QString text;
        for(const auto& block : blocks) {
            text.append(block.text);
        }
        return text;
    }
};

struct RichScript
{
    QString script;
    RichText text;

    bool operator==(const RichScript& other) const
    {
        return std::tie(script, text) == std::tie(other.script, other.text);
    };

    friend QDataStream& operator<<(QDataStream& stream, const RichScript& richScript)
    {
        stream << richScript.script;
        return stream;
    }

    friend QDataStream& operator>>(QDataStream& stream, RichScript& richScript)
    {
        stream >> richScript.script;
        return stream;
    }
};
} // namespace Fooyin
