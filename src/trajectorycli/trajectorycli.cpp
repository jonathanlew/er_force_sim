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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <clocale>
#include <QtGlobal>
#include <QDebug>

#include "common.h"
#include "core/protobuffilereader.h"
#include "src/protobuf/pathfinding.pb.h"


// IO
static Vector deserializeVector(const pathfinding::Vector &v)
{
    Vector result(0, 0);
    if (v.has_x()) result.x = v.x();
    if (v.has_y()) result.y = v.y();
    return result;
}

static TrajectoryInput deserializeTrajectoryInput(const pathfinding::TrajectoryInput &input)
{
    TrajectoryInput result;
    if (input.has_v0()) {
        result.v0 = deserializeVector(input.v0());
    }
    if (input.has_v1()) {
        result.v1 = deserializeVector(input.v1());
    }
    if (input.has_s0()) {
        result.s0 = deserializeVector(input.s0());
    }
    if (input.has_s1()) {
        result.s1 = deserializeVector(input.s1());
    }
    if (input.has_max_speed()) {
        result.maxSpeed = input.max_speed();
    }
    if (input.has_acceleration()) {
        result.acceleration = input.acceleration();
    }

    result.distance = result.s1 - result.s0;
    result.exponentialSlowDown = result.v1 == Vector(0, 0);
    result.maxSpeedSquared = result.maxSpeed * result.maxSpeed;

    return result;
}


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("Trajectory-CLI");
    app.setOrganizationName("ER-Force");

    std::setlocale(LC_NUMERIC, "C");

    QCommandLineParser parser;
    parser.setApplicationDescription("Trajectory preprocessing");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "Pathfinding input file to read");

    QCommandLineOption standardSampler("s", "Optimize the standard sampler intermediate positions", "output file name");
    parser.addOption(standardSampler);
    QCommandLineOption endInObstacle("e", "Optimize the end in obstacle sampler search parameters");
    parser.addOption(endInObstacle);
    QCommandLineOption alphaTime("a", "Optimize the alpha time trajectory search parameters");
    parser.addOption(alphaTime);

    // parse command line
    parser.process(app);

    int argCount = parser.positionalArguments().size();
    if (argCount != 1) {
        parser.showHelp(1);
        return 0;
    }

    if (!parser.isSet(standardSampler) && !parser.isSet(endInObstacle) && !parser.isSet(alphaTime)) {
        qDebug() <<"At lest one optimizer must be run!";
        parser.showHelp(1);
        return 0;
    }

    QString path = parser.positionalArguments().first();

    std::vector<Situation> situations;

    ProtobufFileReader reader;
    if (!reader.open(path, "KHONSU PATHFINDING LOG")) {
        qDebug() <<"Could not open file:"<<path;
        exit(1);
    }

    std::cout <<"Loading situations"<<std::endl;

    pathfinding::PathFindingTask situation;
    pathfinding::InputSourceType sourceSoFar = pathfinding::None;
    while (reader.readNext(situation)) {
        Situation s;
        if (situation.has_state()) {
            s.world.deserialize(situation.state());
        }
        if (situation.has_input()) {
            s.input = deserializeTrajectoryInput(situation.input());
        }
        if (situation.has_type()) {
            s.sourceType = situation.type();
        } else {
            s.sourceType = pathfinding::AllSamplers;
        }
        // check for properly behaved pathfinding input files, as recordings can be mixed
        if (sourceSoFar != pathfinding::None && sourceSoFar != s.sourceType) {
            std::cerr <<"Error: mixed pathfinding input sources in the input file"<<std::endl;
            exit(1);
        }
        sourceSoFar = s.sourceType;
        situations.push_back(s);
        situation.Clear();
    }

    std::cout <<"Number of situations loaded: "<<situations.size()<<std::endl;

    if (parser.isSet(standardSampler)) {
        std::cout <<"Optimizing standard sampler intermediate points"<<std::endl;
        if (sourceSoFar != pathfinding::StandardSampler) {
            std::cerr <<"Error: trying to use pathfinding inputs not collected for the standard sampler!"<<std::endl;
            exit(1);
        }
        optimizeStandardSamplerPoints(situations, parser.value(standardSampler));
    }

    if (parser.isSet(endInObstacle)) {
        std::cout <<"Optimizing end in obstacle sampler"<<std::endl;
        if (sourceSoFar != pathfinding::EndInObstacleSampler) {
            std::cerr <<"Error: trying to use pathfinding inputs not collected for the end in obstacle sampler!"<<std::endl;
            exit(1);
        }
        optimizeEndInObstacleParameters(situations);
    }

    if (parser.isSet(alphaTime)) {
        std::cout <<"Optimizing alpha time trajectory generation"<<std::endl;
        if (sourceSoFar != pathfinding::AllSamplers) {
            std::cerr <<"Error: trying to use pathfinding inputs not collected for the whole trajectorypath!"<<std::endl;
            exit(1);
        }
        optimizeAlphaTimeTrajectoryParameters(situations);
    }

    return 0;
}
