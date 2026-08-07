add_executable(header_generator generator.cpp)
set(GENERATED_HEADER ${CMAKE_BINARY_DIR}/generated/table.h)

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
