/*
 * Fooyin
 * Copyright © 2022, Luke Taylor <LukeT1@proton.me>
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

#include <QMainWindow>
#include <QPointer>

namespace Fooyin {
class ActionManager;
class MainMenuBar;
class SettingsManager;
class StatusWidget;
class Track;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum StartupBehaviour : uint8_t
    {
        StartNormal = 0,
        StartMaximised,
        StartHidden,
        StartPrev
    };
    Q_ENUM(StartupBehaviour)

    enum WindowState : uint8_t
    {
        Normal = 0,
        Maximised,
        Hidden
    };
    Q_ENUM(WindowState)

    explicit MainWindow(ActionManager* actionManager, MainMenuBar* menubar, SettingsManager* settings,
                        QWidget* parent = nullptr);
    ~MainWindow() override;

    void open();
    void raiseWindow();
    void toggleVisibility();
    void exit();

    void setTitle(const QString& title);
    void resetTitle();

    void installStatusWidget(StatusWidget* statusWidget);

    [[nodiscard]] QSize sizeHint() const override;

protected:
    bool event(QEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    WindowState currentState();
    void saveWindowGeometry();
    void restoreWindowGeometry();
    void restoreState(WindowState state);
    void hideToTray(bool hide);

    MainMenuBar* m_mainMenu;
    SettingsManager* m_settings;
    QPointer<StatusWidget> m_statusWidget;

    WindowState m_prevState;
    WindowState m_state;
    bool m_isHiding;
    bool m_hasQuit;
    bool m_showStatusTips;
};
} // namespace Fooyin
