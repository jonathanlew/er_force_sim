/***************************************************************************
 *   Copyright 2020 Michel Schmid
 *   Robotics Erlangen e.V.                                                *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef REFEREEINFOWIDGET_H
#define REFEREEINFOWIDGET_H

#include "src/protobuf/status.h"
#include <QStringList>
#include <QWidget>

namespace Ui {
    class RefereeInfoWidget;
}

class RefereeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RefereeInfoWidget(QWidget *parent = 0);
    ~RefereeInfoWidget() override;
    RefereeInfoWidget(const RefereeInfoWidget&) = delete;
    RefereeInfoWidget& operator=(const RefereeInfoWidget&) = delete;

public slots:
    void handleStatus(const Status &status);
    void setStyleSheets(bool useDark);

private:
    static QString createStyleSheet(const QColor &color);

private:
    Ui::RefereeInfoWidget *ui;

    uint m_yellowKeeperId = 0;
    uint m_blueKeeperId = 0;

    uint m_yellowYellowCards = 0;
    uint m_blueYellowCards = 0;

    bool m_yellowTimerZero = true;
    bool m_blueTimerZero = true;
};

#endif // REFEREEINFOWIDGET_H
