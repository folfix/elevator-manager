cmake_minimum_required(VERSION 3.10)
project(ElevatorManager)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/home/jan/Qt/5.12.2/gcc_64/lib/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Concurrent REQUIRED)

file(GLOB ElevatorManager_SRC
        "*.h"
        "*.cpp"
        )

add_executable(ElevatorManager ${ElevatorManager_SRC})

target_link_libraries(ElevatorManager Qt5::Core)
target_link_libraries(ElevatorManager Qt5::Widgets)
target_link_libraries(ElevatorManager Qt5::Quick)
target_link_libraries(ElevatorManager Qt5::Concurrent)

add_subdirectory(tests)