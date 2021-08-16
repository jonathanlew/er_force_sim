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

#ifndef ROBOTFILTER_H
#define ROBOTFILTER_H

#include "filter.h"
#include "kalmanfilter.h"
#include "src/protobuf/robot.pb.h"
#include "src/protobuf/ssl_detection.pb.h"
#include "src/protobuf/world.pb.h"
#include "src/core/fieldtransform.h"
#include <QList>
#include <QMap>
#include <QPair>

class SSL_DetectionRobot;

struct RobotInfo {
    Eigen::Vector2f robotPos = Eigen::Vector2f(0, 0);
    Eigen::Vector2f dribblerPos = Eigen::Vector2f(0, 0);
    bool chipCommand = false;
    bool linearCommand = false;
    int identifier = std::numeric_limits<int>::max(); // uniquely identifies each robot, stays constant over time (includes id and team)
};

class RobotFilter : public Filter
{
public:
    RobotFilter(const SSL_DetectionRobot &robot, qint64 lastTime, bool teamIsYellow);
    ~RobotFilter() override;
    RobotFilter(const RobotFilter&) = delete;
    RobotFilter& operator=(const RobotFilter&) = delete;

    void update(qint64 time);
    void get(world::Robot *robot, const FieldTransform &transform, bool noRawData);

    void addVisionFrame(qint32 cameraId, const SSL_DetectionRobot &robot, qint64 time, qint64 visionProcessingTime);
    void addRadioCommand(const robot::Command &radioCommand, qint64 time);

    float distanceTo(const SSL_DetectionRobot &robot) const;
    RobotInfo getRobotInfo() const;

private:
    struct VisionFrame
    {
        VisionFrame(qint32 cameraId, const SSL_DetectionRobot &detection, qint64 time, qint64 vPT)
            : cameraId(cameraId), detection(detection), time(time), visionProcessingTime(vPT) {}
        qint32 cameraId;
        SSL_DetectionRobot detection;
        qint64 time;
        qint64 visionProcessingTime;
    };
    typedef QPair<robot::Command, qint64> RadioCommand;
    typedef KalmanFilter<6, 3> Kalman;

    void resetFutureKalman();
    void predict(qint64 time, bool updateFuture, bool permanentUpdate, bool cameraSwitched, const RadioCommand &cmd);
    void applyVisionFrame(const VisionFrame &frame);
    void invalidateRobotCommand(qint64 time);
    double limitAngle(double angle) const;

    uint m_id;
    bool m_teamIsYellow;
    // for debugging
    QMap<int, world::RobotPosition> m_lastRaw;
    QList<world::RobotPosition> m_measurements;

    Kalman *m_kalman;
    // m_lastTime is inherited from Filter
    Kalman *m_futureKalman;
    qint64 m_futureTime;
    RadioCommand m_lastRadioCommand;
    RadioCommand m_futureRadioCommand;
    QList<VisionFrame> m_visionFrames;
    QList<RadioCommand> m_radioCommands;
};

#endif // ROBOTFILTER_H
