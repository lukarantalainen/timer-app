cmake_minimum_required(VERSION 4.0)

project(TimerApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(generator)
add_subdirectory(client)
add_subdirectory(server)
