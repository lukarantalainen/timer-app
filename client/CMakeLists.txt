add_executable(client
  main.cpp
  mainwindow.cpp
  timer.cpp
  logdisplay.cpp
  keyboard_key.cpp
  keyboard_heatmap.cpp
  client.cpp
)


add_dependencies(client generate_headers)

target_link_libraries(client PRIVATE Qt6::Widgets)

target_include_directories(client PRIVATE
  ${CMAKE_BINARY_DIR}/generated
  ${CMAKE_SOURCE_DIR}/include
)
