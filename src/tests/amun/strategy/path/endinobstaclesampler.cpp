/***************************************************************************
 *   Copyright 2020 Andreas Wendler                                        *
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

#include "gtest/gtest.h"
#include "src/core/rng.h"
#include "path/endinobstaclesampler.h"
#include "path/worldinformation.h"
#include "path/alphatimetrajectory.h"

#include <functional>

static WorldInformation constructWorld() {
    WorldInformation world;
    world.setRadius(0.08f);
    world.setBoundary(-10, -10, 10, 10); // just some dummy field
    world.setOutOfFieldObstaclePriority(50);
    world.setRobotId(0);
    world.clearObstacles();
    return world;
}

static TrajectoryInput constructBasicInput(Vector s0, Vector s1) {
    TrajectoryInput input;
    input.v0 = Vector(0, 0);
    input.v1 = Vector(0, 0);
    input.distance = s1 - s0;
    input.s0 = s0;
    input.s1 = s1;
    input.t0 = 0;
    input.exponentialSlowDown = input.v1 == Vector(0, 0);
    input.maxSpeed = 3;
    input.maxSpeedSquared = input.maxSpeed * input.maxSpeed;
    input.acceleration = 3.5;
    return input;
}

static Vector optimizeCloseness(std::function<void(WorldInformation&)> obstacleAdder,
                                const TrajectoryInput &input)
{
    const int ITERATIONS = 200;

    WorldInformation world = constructWorld();
    obstacleAdder(world);
    world.collectObstacles();
    world.collectMovingObstacles();

    PathDebug debug;
    RNG rng(1);

    EndInObstacleSampler sampler(&rng, world, debug);

    // is equivalent to having more samples in the compute function -> should be close to the optimal solution
    for (int i = 0;i<ITERATIONS;i++) {
        sampler.compute(input);
    }

    auto result = sampler.getResult();

    if (result.size() == 0) {
        return Vector(0, 0);
    }

    // check if the trajectory intersects any obstacles
    Vector offset = input.s0;
    float timeOffset = 0;
    for (const auto &info : result) {
        if (world.isTrajectoryInObstacle(info.profile, timeOffset, offset)) {
            // ASSERT_FALSE does not work here since a return value is necessary
            return Vector(0, 0);
        }
        offset += info.profile.endPos();
        timeOffset += info.profile.time();
    }

    return result[0].desiredDistance;
}

// tests the complete optimization
TEST(EndInObstacleSampler, WholeSampling) {
    {
        Vector s0(1, 1);
        Vector s1(5, 5);
        TrajectoryInput input = constructBasicInput(s0, s1);
        auto addObst = [&](WorldInformation &world) {
            world.addCircle(s1.x, s1.y, 2, "circle around target", 50);
        };

        Vector target = optimizeCloseness(addObst, input) + s0;

        ASSERT_GE(s1.distance(target), 2);
        ASSERT_LE(s1.distance(target), 2.1);

        // TODO: enable this test if the sampler also optimizes the time to the target pos
        // ASSERT_LE(target.distance(Vector(3, 3)), 0.2);
    }

    {
        Vector s0(1, 1);
        Vector s1(5, 5);
        TrajectoryInput input = constructBasicInput(s0, s1);
        auto addObst = [&](WorldInformation &world) {
            world.addRect(s1.x - 0.3, 10, 10, -10, "rect around target", 50, 0);
        };

        Vector target = optimizeCloseness(addObst, input) + s0;
        Vector desiredTarget = s1 + Vector(-0.3, 0);
        ASSERT_LE(desiredTarget.distance(target), 0.1);
    }

    // test with a moving obstacle that has a large end time
    {
        Vector s0(1, 1);
        Vector s1(5, 5);
        TrajectoryInput input = constructBasicInput(s0, s1);
        auto addObst = [&](WorldInformation &world) {
            world.addCircle(s1.x, s1.y, 1, "circle around target", 50);
            world.addMovingCircle(s1, Vector(0, 0), Vector(0, 0), 0, 100, 1.5, 50);
        };

        Vector target = optimizeCloseness(addObst, input) + s0;

        ASSERT_GE(s1.distance(target), 1.5);
        ASSERT_LE(s1.distance(target), 1.6);
    }
}
