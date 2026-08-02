
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type not set. We'll default to Debug.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
else()
  message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")
endif()

set(VALID_BUILD_TYPES Debug Release RelWithDebInfo MinSizeRel)
if(NOT CMAKE_BUILD_TYPE IN_LIST VALID_BUILD_TYPES)
  message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
endif()

function(target_set_standards target)

  set_target_properties(${target} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )

  target_compile_options(${target} PRIVATE
    -Wall -Wextra -Wshadow -Wundef
    -Wimplicit-function-declaration
    -Wredundant-decls
    -Wstrict-prototypes
    -fno-common
    -MD
    $<$<CONFIG:Debug>:
      -Og -g
    >
    $<$<CONFIG:RelWithDebInfo>:
      -Og -g -DNDEBUG
    >
    $<$<CONFIG:Release>:
      -Os -DNDEBUG
    >
    $<$<CONFIG:MinSizeRel>:
      -Os -DNDEBUG
    >
  )

  target_link_options(${target} PRIVATE
    -nostartfiles
    -Wl,-gc-sections
    -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
  )

  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE_NAME:${target}>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${target}>
    COMMENT "Generating size information for $<TARGET_FILE_NAME:${target}>"
    VERBATIM
  )

  target_link_options(${target} PRIVATE
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${target}.map -Wl,--cref
  )
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND "" # We just want the post-build comment.
    COMMENT "Generating $<TARGET_FILE_BASE_NAME:${target}>.map"
  )

  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
      $<TARGET_FILE:${target}>
      $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.bin
    COMMENT "Generating $<TARGET_FILE_BASE_NAME:${target}>.bin"
    VERBATIM
  )

  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D
      $<TARGET_FILE:${target}>
      > $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.lst
    COMMENT "Generating $<TARGET_FILE_BASE_NAME:${target}>.lst"
    VERBATIM
  )

endfunction()
