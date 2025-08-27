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

#pragma once

#include <algorithm>
#include <ranges>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

#include <QRegularExpression>
#include <QString>

namespace Fooyin::Utils {
template <typename Cntr, typename Element>
int findIndex(const Cntr& c, const Element& e)
{
    int index  = -1;
    auto eIter = std::ranges::find(std::as_const(c), e);
    if(eIter != c.cend()) {
        index = static_cast<int>(std::ranges::distance(c.cbegin(), eIter));
    }
    return index;
}

template <typename Element>
bool contains(const std::vector<Element>& c, const Element& f)
{
    return std::ranges::find(std::as_const(c), f) != c.cend();
}

template <typename T, typename Hash, typename Cntr>
Cntr intersection(const Cntr& v1, const Cntr& v2)
{
    Cntr result;
    std::unordered_set<T, Hash> first{v1.cbegin(), v1.cend()};

    auto commonElements = v2 | std::views::filter([&first](const auto& elem) { return first.contains(elem); });

    for(const auto& entry : commonElements) {
        result.push_back(entry);
    }
    return result;
}

template <typename Cntr, typename Pred>
Cntr filter(const Cntr& container, Pred pred)
{
    Cntr result;
    std::ranges::copy_if(std::as_const(container), std::back_inserter(result), pred);
    return result;
}

template <typename Cntr>
Cntr sortByIndexes(const Cntr& target, const std::vector<int>& indexes)
{
    Cntr sorted(indexes.size());

    for(size_t i{0}; i < indexes.size(); ++i) {
        sorted[i] = target[indexes.at(i)];
    }

    return sorted;
}

template <typename T>
void move(std::vector<T>& v, size_t from, size_t to)
{
    if(from == to) {
        return;
    }

    if(from > to) {
        std::rotate(v.rend() - from - 1, v.rend() - from, v.rend() - to);
    }
    else {
        std::rotate(v.begin() + from, v.begin() + from + 1, v.begin() + to + 1);
    }
}

template <typename T>
std::optional<T> take(std::vector<T>& vec, std::size_t index)
{
    if(index >= vec.size()) {
        return {};
    }

    T element = std::move(vec.at(index));
    vec.erase(vec.begin() + index);
    return element;
}

template <typename T, typename StringExtractor>
QString findUniqueString(const QString& name, const T& elements, StringExtractor&& extractor)
{
    if(name.isEmpty()) {
        return {};
    }

    std::set<QString> existingNames;
    for(const auto& element : elements) {
        existingNames.emplace(extractor(element));
    }

    if(!existingNames.contains(name)) {
        return name;
    }

    auto findCount = [&existingNames](const QString& str) -> int {
        const QString regexName    = QRegularExpression::escape(str);
        const QString regexPattern = QString::fromUtf8(R"(^%1\s*(\(\d+\))?\s*$)").arg(regexName);
        const QRegularExpression pattern{regexPattern};

        return static_cast<int>(std::ranges::count_if(
            existingNames, [&pattern](const auto& element) { return pattern.match(element).hasMatch(); }));
    };

    const int count = findCount(name);

    return count > 0 ? QStringLiteral("%1 (%2)").arg(name).arg(count) : name;
}

template <typename T>
class asRange
{
public:
    explicit asRange(T& data)
        : m_data{data}
    { }

    auto begin()
    {
        return m_data.keyValueBegin();
    }

    auto end()
    {
        return m_data.keyValueEnd();
    }

private:
    T& m_data;
};
} // namespace Fooyin::Utils
