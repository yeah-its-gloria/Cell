# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(OpenInputRuntime LANGUAGES C CXX VERSION 1.0.0)

add_executable(OpenInputRuntime
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Server.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Server.cc

               ${CMAKE_CURRENT_LIST_DIR}/Sources/DeviceThread.cc

               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Result.hh

# Sony PlayStation 5 DualSense Controller

               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/DualSense.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/DualSense/DualSense.cc

# Sony PlayStation 3 Controller

               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/PS3.hh

# Nintendo GameCube Controller (via a Nintendo Wii U GameCube Controller Adapter)

               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/GameCubeAdapter.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/GameCubeAdapter/GameCubeAdapter.cc

# Nintendo Wiimote

               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Find.cc
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Poll.cc
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/SetUpReporting.cc
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Wiimote.cc

               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/Buttons.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/ClassicController.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/ClassicControllerButtons.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/LED.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/Nunchuck.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/NunchuckButtons.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/Reports.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/Requests.hh
               ${CMAKE_CURRENT_LIST_DIR}/Sources/Devices/Wiimote/Communications/StatusState.hh
)

target_include_directories(OpenInputRuntime PRIVATE ${OPENINPUT_PRIVATE_INCLUDE_DIR})
target_link_libraries(OpenInputRuntime PRIVATE CellCore OpenInput)

Cell_Platform_Bootstrap_Target(OpenInputRuntime TRUE)
