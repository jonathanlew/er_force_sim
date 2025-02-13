# ***************************************************************************
# *   Copyright 2019 Andreas Wendler                                        *
# *   Robotics Erlangen e.V.                                                *
# *   http://www.robotics-erlangen.de/                                      *
# *   info@robotics-erlangen.de                                             *
# *                                                                         *
# *   This program is free software: you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation, either version 3 of the License, or     *
# *   any later version.                                                    *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
# ***************************************************************************

add_library(gamecontroller
    include/gamecontroller/externalgamecontroller.h
    include/gamecontroller/gamecontrollerconnection.h
    include/gamecontroller/sslgamecontroller.h

    externalgamecontroller.cpp
    gamecontrollerconnection.cpp
    sslgamecontroller.cpp
    sslvisiontracked.cpp
    sslvisiontracked.h
)
target_link_libraries(gamecontroller
    PUBLIC shared::core
    PUBLIC shared::protobuf
    PUBLIC Qt5::Core
    PUBLIC Qt5::Network
)
target_include_directories(gamecontroller
    INTERFACE include
    PRIVATE include/gamecontroller
)
add_dependencies(gamecontroller
    gamecontroller_download
)
target_compile_definitions(gamecontroller PRIVATE GAMECONTROLLER_EXECUTABLE_LOCATION="${GAMECONTROLLER_EXECUTABLE_LOCATION}")


add_library(amun::gamecontroller ALIAS gamecontroller)
