cmake_minimum_required(VERSION 3.10)
project(ElevatorManagerTests)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/home/jan/Qt/5.12.2/gcc_64/lib/cmake")

find_package(Threads REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Concurrent REQUIRED)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB ElevatorManager_TESTS
        "*.h"
        "*.cpp"
        "../src/elevator/elevator.h"
        "../src/elevator/elevator.cpp"
        "../src/passenger/passenger.h"
        "../src/passenger/passenger.cpp"
        )

add_executable(ElevatorManagerTests ${ElevatorManager_TESTS})

#include_directories(../)
#link_directories(../)

target_link_libraries(ElevatorManagerTests gtest gtest_main)
target_link_libraries(ElevatorManagerTests Threads::Threads)
target_link_libraries(ElevatorManagerTests Qt5::Core)
target_link_libraries(ElevatorManagerTests Qt5::Widgets)
target_link_libraries(ElevatorManagerTests Qt5::Quick)
target_link_libraries(ElevatorManagerTests Qt5::Concurrent)

#target_link_libraries(ElevatorManagerTests ElevatorManager)
#target_link_directories(ElevatorManagerTests PUBLIC ../)
#link_directories(../)

#target_link_libraries(ElevatorManagerTests ElevatorManagerLib)
