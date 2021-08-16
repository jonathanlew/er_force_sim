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

#ifndef BALLFILTER_H
#define BALLFILTER_H

#include "filter.h"
#include "src/core/fieldtransform.h"
#include "kalmanfilter.h"
#include "abstractballfilter.h"
#include "src/protobuf/debug.pb.h"
#include "src/protobuf/world.pb.h"

class FlyFilter;
class GroundFilter;
class DribbleFilter;

class BallTracker : public Filter
{
public:
    BallTracker(const SSL_DetectionBall &ball, qint64 last_time, qint32 primaryCamera, CameraInfo* cameraInfo, RobotInfo robotInfo, qint64 visionProcessingDelay);
    BallTracker(const BallTracker& previousFilter, qint32 primaryCamera);
    ~BallTracker() override;
    BallTracker(const BallTracker&) = delete;
    BallTracker& operator=(const BallTracker&) = delete;

public:
    void update(qint64 time);
    void updateConfidence();
    void get(world::Ball *ball, const FieldTransform &transform, bool resetRaw); // writes to world state
    void addVisionFrame(const SSL_DetectionBall& ball, qint64 time, qint32 cameraId, RobotInfo robotInfo, qint64 visionProcessingDelay);
    bool acceptDetection(const SSL_DetectionBall& ball, qint64 time, qint32 cameraId, RobotInfo robotInfo, qint64 visionProcessingDelay);
    void calcDistToCamera(bool flying);
    float cachedDistToCamera();
    bool isFlying() const;
    qint64 initTime() const { return m_initTime; }
    bool isShot() const;
    double confidence() const { return m_confidence; }
    int rawBallCount() const { return m_rawBallCount; } // basically reports the age of the filter in the number of accepted raw ball detections
#ifdef ENABLE_TRACKING_DEBUG
    const amun::DebugValues &debugValues() const { return m_debug; }
    void clearDebugValues() {
        m_debug.clear_value();
        m_debug.clear_visualization();
        m_debug.clear_log();
        m_debug.clear_plot();
    }
#endif

private:  
    qint64 m_lastUpdateTime;
    GroundFilter* m_groundFilter;
    FlyFilter *m_flyFilter;
    DribbleFilter *m_dribbleFilter;
    QList<VisionFrame> m_visionFrames;
    QList<VisionFrame> m_rawMeasurements;
    CameraInfo* m_cameraInfo;
    qint64 m_initTime;
    Eigen::Vector2f m_lastBallPos;
    qint64 m_lastFrameTime;
    double m_confidence;
    int m_updateFrameCounter;
    float m_cachedDistToCamera;
    int m_rawBallCount = 0;

#ifdef ENABLE_TRACKING_DEBUG
    amun::DebugValues m_debug;
    void debug(const char* key, float value){
        amun::DebugValue *debugValue = m_debug.add_value();
        std::string str = (QString::number(m_primaryCamera)+QString("/")+QString(key)).toStdString();
        debugValue->set_key(str);
        debugValue->set_float_value(value);
    }
    void debug(const char* key, const char* value){
        amun::DebugValue *debugValue = m_debug.add_value();
        std::string str = (QString::number(m_primaryCamera)+QString("/")+QString(key)).toStdString();
        debugValue->set_key(str);
        debugValue->set_string_value(value);
    }
#else
    void debug(const char* key, float value) const {}
    void debug(const char* key, const char* value) const {}
#endif
};

#endif // BALLFILTER_H
