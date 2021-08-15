/***************************************************************************
 *   Copyright 2015 Michael Eischer, Philipp Nordhus                       *
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

#ifndef REFEREE_H
#define REFEREE_H

#include "src/protobuf/gamestate.pb.h"
#include "src/protobuf/ssl_referee.pb.h"
#include "src/protobuf/world.pb.h"
#include <QByteArray>
#include <QObject>
#include <memory>

class Referee : public QObject
{
    Q_OBJECT

public:
    explicit Referee();

public:
    const amun::GameState& gameState() const { return m_gameState; }
    bool getFlipped() const { return m_flipped; }

public slots:
    void handlePacket(const QByteArray &data);
    void process(const world::State &worldState);
    bool isGameRunning();

private:
    static amun::GameState::State processCommand(SSL_Referee::Command command, amun::GameState::State currentState);
    void setFlipped(bool flipped);

private:
    amun::GameState m_gameState;
    world::Ball m_ball;
    quint32 m_counter;
    bool m_flipped;
    std::optional<SSL_Referee::Command> m_lastCommand;
};

#endif // REFEREE_H
