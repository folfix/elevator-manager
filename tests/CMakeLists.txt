cmake_minimum_required(VERSION 3.10)
project(ElevatorManager)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/home/jan/Qt/5.12.2/gcc_64/lib/cmake")

find_package(Threads REQUIRED)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB ElevatorManager_TESTS
        "*.h"
        "*.cpp"
        )

add_executable(ElevatorManagerTests ${ElevatorManager_TESTS})

target_link_libraries(ElevatorManagerTests gtest gtest_main)
target_link_libraries(ElevatorManagerTests Threads::Threads)
