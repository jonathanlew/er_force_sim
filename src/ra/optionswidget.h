/***************************************************************************
 *   Copyright 2016 Michael Eischer                                        *
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

#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QSet>
#include <QWidget>
#include "src/protobuf/command.h"
#include "src/protobuf/status.h"

class QStandardItem;
class QStandardItemModel;

namespace Ui {
class OptionsWidget;
}

class OptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsWidget(QWidget *parent = 0);
    ~OptionsWidget() override;
    OptionsWidget(const OptionsWidget&) = delete;
    OptionsWidget& operator=(const OptionsWidget&) = delete;

signals:
    void sendCommand(const Command &command);

public slots:
    void handleStatus(const Status &status);

private slots:
    void itemChanged(QStandardItem *item);

private:
    void sendItemChanged(const QString &name, bool value);
    void handleAmunState(const amun::StatusAmun &strategy);

    typedef QHash<QByteArray, QStandardItem*> HashMap;
    Ui::OptionsWidget *ui;
    QStandardItemModel *m_model;
    HashMap m_items;
    QSet<QString> m_selection;
};

#endif // OPTIONSWIDGET_H
