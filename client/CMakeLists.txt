set(CMAKE_AUTORCC ON)

qt_add_executable(client
  main.cpp
  mainwindow.cpp
  timer.cpp
  logdisplay.cpp
  client.cpp
  statusbar.cpp
  database.cpp
  menubar.cpp
  application.qrc
  keyboard/keyboard_heatmap.cpp
  keyboard/keyboard_key.cpp
  keyboard/keyboard.cpp

  include/client.h
  include/database.h
  include/keyboard.h
  include/logdisplay.h
  include/mainwindow.h
  include/menubar.h
  include/statusbar.h
  include/timer.h
)

add_dependencies(client generate_headers)

find_package(SQLite3)
target_link_libraries(client PRIVATE Qt6::Widgets Qt6::Network SQLite3::SQLite3)

target_include_directories(client PRIVATE
  ${CMAKE_BINARY_DIR}/generated
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
