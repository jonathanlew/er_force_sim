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

#include "escapeobstaclesampler.h"
#include "src/core/rng.h"

bool EscapeObstacleSampler::TrajectoryRating::isBetterThan(const TrajectoryRating &other)
{
    if (endsSafely && !other.endsSafely) {
        return true;
    } else if (!endsSafely && other.endsSafely) {
        return false;
    }
    if (maxPrio < other.maxPrio) {
        return true;
    } else if (maxPrio > other.maxPrio) {
        return false;
    }
    if (maxPrioTime < other.maxPrioTime) {
        return true;
    }
    return false;
}

bool EscapeObstacleSampler::compute(const TrajectoryInput &input)
{
    // first stage: find a path that quickly exists all obstacles
    // the second stage is executed by the regular standard sampler
    {
        // try last frames trajectory
        SpeedProfile bestProfile = AlphaTimeTrajectory::calculateTrajectory(input.v0, Vector(0, 0), m_bestEscapingTime, m_bestEscapingAngle,
                                                                            input.acceleration, input.maxSpeed, 0, false);
        auto bestRating = rateEscapingTrajectory(input, bestProfile);
        for (int i = 0;i<25;i++) {
            float time, angle;
            if (m_rng->uniformInt() % 2 == 0) {
                // random sampling
                if (!bestRating.endsSafely) {
                    time = m_rng->uniformFloat(0.001f, 6.0f);
                } else {
                    time = m_rng->uniformFloat(0.001f, 2.0f);
                }
                angle = m_rng->uniformFloat(0, float(2 * M_PI));
            } else {
                // sample around current best point
                time = std::max(0.001f, m_bestEscapingTime + m_rng->uniformFloat(-0.1f, 0.1f));
                angle = m_bestEscapingAngle + m_rng->uniformFloat(-0.1f, 0.1f);
            }

            SpeedProfile profile = AlphaTimeTrajectory::calculateTrajectory(input.v0, Vector(0, 0), time, angle, input.acceleration, input.maxSpeed, 0, false);
            auto rating = rateEscapingTrajectory(input, profile);
            if (rating.isBetterThan(bestRating)) {
                bestRating = rating;
                bestProfile = profile;
                m_bestEscapingTime = time;
                m_bestEscapingAngle = angle;
            }
        }
        m_maxIntersectingObstaclePrio = bestRating.maxPrio;

        m_generationInfo.clear();
        if (!bestRating.endsSafely) {
            return false;
        }
        TrajectoryGenerationInfo info;
        bestProfile.limitToTime(bestRating.escapeTime);
        info.profile = bestProfile;
        info.desiredDistance = bestProfile.endPos();
        m_generationInfo.push_back(info);
    }
    return true;
}

void EscapeObstacleSampler::updateFrom(const EscapeObstacleSampler &other)
{
    m_bestEscapingTime = other.m_bestEscapingTime;
    m_bestEscapingAngle = other.m_bestEscapingAngle;
}

auto EscapeObstacleSampler::rateEscapingTrajectory(const TrajectoryInput &input, const SpeedProfile &speedProfile) const -> TrajectoryRating
{
    const float OUT_OF_OBSTACLE_TIME = 0.1f;
    const float LONG_OUF_OF_OBSTACLE_TIME = 1.5f; // used when the trajectory has not yet intersected any obstacle
    const float SAMPLING_INTERVAL = 0.03f;

    const float totalTime = speedProfile.time();
    int samples = int(totalTime / SAMPLING_INTERVAL) + 1;

    TrajectoryRating result;

    int goodSamples = 0;
    float fineTime = totalTime;
    for (int i = 0;i<samples;i++) {
        float time = i * SAMPLING_INTERVAL;

        Vector pos = speedProfile.positionAndSpeedForTime(time).first + input.s0;
        int obstaclePriority = -1;
        if (!m_world.pointInPlayfield(pos, m_world.radius())) {
            obstaclePriority = m_world.outOfFieldPriority();
        }
        for (const auto obstacle : m_world.obstacles()) {
            if (obstacle->prio > obstaclePriority) {
                float distance = obstacle->distance(pos);
                if (result.maxPrio == -1) {
                    // when the trajectory does not intersect any obstacles, we want to stay as far away as possible from them
                    result.maxPrioTime = std::min(result.maxPrioTime, distance);
                }
                if (distance < 0) {
                    obstaclePriority = obstacle->prio;
                }
            }
        }
        for (const auto o : m_world.movingObstacles()) {
            if (o->prio > obstaclePriority && o->intersects(pos, time + input.t0)) {
                obstaclePriority = o->prio;
            }
        }
        if (obstaclePriority == -1) {
            goodSamples++;
            float boundaryTime = result.maxPrio >= 0 ? OUT_OF_OBSTACLE_TIME : LONG_OUF_OF_OBSTACLE_TIME;
            if (goodSamples > boundaryTime * (1.0f / SAMPLING_INTERVAL) && fineTime == totalTime) {
                fineTime = time;
            }
            if (goodSamples > LONG_OUF_OF_OBSTACLE_TIME * (1.0f / SAMPLING_INTERVAL)) {
                result.endsSafely = true;
                break;
            }
        } else {
            goodSamples = 0;
        }
        if (obstaclePriority > result.maxPrio) {
            result.maxPrio = obstaclePriority;
            result.maxPrioTime = 0;
        }
        if (obstaclePriority == result.maxPrio) {
            if (i == samples-1) {
                // strong penalization for stopping in an obstacle
                result.maxPrioTime += 10;
            } else {
                result.maxPrioTime += SAMPLING_INTERVAL;
            }
        }
    }
    if (result.maxPrio == -1) {
        result.escapeTime = OUT_OF_OBSTACLE_TIME;
    } else {
        result.escapeTime = fineTime;
    }
    if (goodSamples > 0 && speedProfile.endSpeed() == Vector(0, 0)) {
        result.endsSafely = true;
    }
    return result;
}
