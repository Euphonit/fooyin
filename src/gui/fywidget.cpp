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

#include <gui/fywidget.h>

#include <utils/crypto.h>

#include <QJsonArray>
#include <QJsonObject>

namespace Fooyin {
FyWidget::FyWidget(QWidget* parent)
    : QWidget{parent}
    , m_id{Utils::generateUniqueHash()}
{ }

Id FyWidget::id() const
{
    return m_id;
}

QString FyWidget::layoutName() const
{
    return name();
}

FyWidget::Features FyWidget::features() const
{
    return m_features;
}

void FyWidget::setFeature(Feature feature, bool on)
{
    if(on) {
        m_features |= feature;
    }
    else {
        m_features &= ~feature;
    }
}

FyWidget* FyWidget::findParent() const
{
    QWidget* parent = parentWidget();
    while(parent && !qobject_cast<FyWidget*>(parent)) {
        parent = parent->parentWidget();
    }
    return qobject_cast<FyWidget*>(parent);
}

QRect FyWidget::widgetGeometry() const
{
    int x = this->x();
    int y = this->y();

    const auto* widget{this};

    while((widget = widget->findParent())) {
        x += widget->x();
        y += widget->y();
    }

    return {x, y, width(), height()};
}

void FyWidget::saveLayout(QJsonArray& layout)
{
    QJsonObject widgetData;
    widgetData[QStringLiteral("ID")] = m_id.name();

    saveLayoutData(widgetData);

    QJsonObject widgetObject;
    widgetObject[layoutName()] = widgetData;

    layout.append(widgetObject);
}

void FyWidget::saveBaseLayout(QJsonArray& layout)
{
    QJsonObject widgetData;

    saveLayoutData(widgetData);

    QJsonObject widgetObject;
    widgetObject[layoutName()] = widgetData;

    layout.append(widgetObject);
}

void FyWidget::loadLayout(const QJsonObject& layout)
{
    if(layout.contains(QStringLiteral("ID"))) {
        m_id = Id{layout[QStringLiteral("ID")].toString()};
    }

    loadLayoutData(layout);
}

void FyWidget::searchEvent(const QString& /*search*/) { }

void FyWidget::layoutEditingMenu(ActionContainer* /*menu*/) { }

void FyWidget::saveLayoutData(QJsonObject& /*object*/) { }

void FyWidget::loadLayoutData(const QJsonObject& /*object*/) { }

void FyWidget::finalise() { }
} // namespace Fooyin

#include "gui/moc_fywidget.cpp"
