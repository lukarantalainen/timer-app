cmake_minimum_required(VERSION 4.0)

project(TimerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(TimerApp
  main.cpp
  mainwindow.cpp
  timer.cpp
  input.cpp
  logdisplay.cpp
  keyboard_key.cpp
  keyboard_heatmap.cpp
)

target_link_libraries(TimerApp PRIVATE Qt6::Widgets)
