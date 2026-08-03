cmake_minimum_required(VERSION 4.0)

project(TimerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(header_generator generator.cpp)
set(GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/generated/table.h)

add_custom_command(
  OUTPUT ${GENERATED_HEADER}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated
  COMMAND header_generator ${GENERATED_HEADER}
  DEPENDS header_generator
  COMMENT "Generating table.h"
)

add_custom_target(generate_headers
  DEPENDS ${GENERATED_HEADER}
)

add_executable(TimerApp
  main.cpp
  mainwindow.cpp
  timer.cpp
  logdisplay.cpp
  keyboard_key.cpp
  keyboard_heatmap.cpp
  client.cpp
)

add_dependencies(TimerApp generate_headers)

target_include_directories(TimerApp PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_link_libraries(TimerApp PRIVATE Qt6::Widgets)
