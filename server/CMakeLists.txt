add_executable(server
  input.cpp
  server.cpp
)

add_dependencies(server generate_headers)

target_include_directories(server PRIVATE
  ${CMAKE_BINARY_DIR}/generated
  ${CMAKE_SOURCE_DIR}/include
)

install(PROGRAMS "$<TARGET_FILE:server>"
  DESTINATION sbin
  RENAME activity-tracker-daemon
)
