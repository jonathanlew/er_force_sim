/***************************************************************************
 *   Copyright 2019 Andreas Wendler                                        *
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

#include "abstractpath.h"
#include "src/core/rng.h"
#include <QDebug>

AbstractPath::AbstractPath(uint32_t rng_seed) :
    m_rng(new RNG(rng_seed))
{
    connect(&m_debug, SIGNAL(gotDebug(amun::DebugValue)), this, SIGNAL(gotDebug(amun::DebugValue)));
    connect(&m_debug, SIGNAL(gotVisualization(amun::Visualization)), this, SIGNAL(gotVisualization(amun::Visualization)));
}

AbstractPath::~AbstractPath()
{
    delete m_rng;
}

void AbstractPath::seedRandom(uint32_t seed)
{
    m_rng->seed(seed);
}

 void AbstractPath::clearObstacles()
 {
     m_world.clearObstacles();
     clearObstaclesCustom();
 }
