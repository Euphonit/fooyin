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

#include "gui/layoutprovider.h"

#include <QDialog>
#include <QItemSelection>

class QListView;
class QPushButton;

namespace Fooyin {
class QuickSetupModel;

class QuickSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuickSetupDialog(LayoutProvider* layoutProvider, QWidget* parent = nullptr);

    [[nodiscard]] QSize sizeHint() const override;

signals:
    void layoutChanged(const Fooyin::FyLayout& layout);

protected:
    void showEvent(QShowEvent* event) override;

private:
    void changeLayout(const QItemSelection& selected, const QItemSelection& deselected);

    QListView* m_layoutList;
    QuickSetupModel* m_model;
    QPushButton* m_accept;
};
} // namespace Fooyin
