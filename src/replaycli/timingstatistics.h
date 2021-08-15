/***************************************************************************
 *   Copyright 2018 Andreas Wendler                                        *
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

#ifndef TIMINGSTATISTICS_H
#define TIMINGSTATISTICS_H

#include <QObject>
#include <QVector>
#include <vector>

#include "src/protobuf/status.h"

class TimingStatistics : public QObject
{
    Q_OBJECT
public:
    TimingStatistics(bool isBlue, bool saveAllData = false, int frames = 0) :
        m_isBlue(isBlue), m_saveAllData(saveAllData) { m_timings.reserve(frames); }
    void printStatistics(bool showHistogram, bool showCumulativeHistogram);

public slots:
    void handleStatus(const Status &status);

private:
    bool m_isBlue;
    bool m_saveAllData;
    int m_counter = 0;
    double m_totalTime = 0.0;
    QVector<int> m_timeHistogram;
    QVector<float> m_timings;
};

#endif // TIMINGSTATISTICS_H
