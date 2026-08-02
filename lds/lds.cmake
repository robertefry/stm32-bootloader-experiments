
function(lds_eval OUT_VAR EXPR)

  set(_probe ${CMAKE_CURRENT_BINARY_DIR}/lds_probe_${OUT_VAR}.c)
  file(WRITE ${_probe} "#include \"lds.h\"\n${EXPR}\n")

  execute_process(
    COMMAND ${CMAKE_C_COMPILER}
      -E -x c -P
      -I${LDS_DIR}
      ${_probe}
    OUTPUT_VARIABLE _result
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE _rc
  )
  if(NOT _rc EQUAL 0)
    message(FATAL_ERROR "lds_eval: preprocessing failed for '${EXPR}'")
  endif()

  string(REGEX REPLACE "[()UL]" "" _result "${_result}")
  string(STRIP "${_result}" _result)

  set(${OUT_VAR} ${_result} PARENT_SCOPE)

endfunction()

function(target_link_script target)

  cmake_parse_arguments(ARG "FLASH" "REGION" "" ${ARGN})
  if(NOT ARG_REGION)
    message(FATAL_ERROR "target_link_script: REGION is required")
  endif()

  if(NOT LDS_DIR)
    message(FATAL_ERROR "target_link_script: LDS_DIR is not set")
  endif()

  set(LDS_HEADER ${LDS_DIR}/lds.h)
  set(LDS_OUT    ${CMAKE_CURRENT_BINARY_DIR}/nucleo-f446re.ld)

  # TODO: Add support for other hardware linker scripts
  set(LDS_SRC    ${LDS_DIR}/nucleo-f446re.ld)

  add_custom_command(
    OUTPUT  ${LDS_OUT}
    COMMAND ${CMAKE_C_COMPILER}
      -E -x c -P
      -I${LDS_DIR}
      -DLD_REGION=${ARG_REGION}
      ${LDS_SRC}
      -o ${LDS_OUT}
    DEPENDS ${LDS_SRC} ${LDS_HEADER}
    COMMENT "Generating linker script for ${target}"
    VERBATIM
  )

  add_custom_target(${target}_ldscript DEPENDS ${LDS_OUT})
  add_dependencies(${target} ${target}_ldscript)

  set_target_properties(${target} PROPERTIES
    LINK_DEPENDS ${LDS_OUT}
  )

  target_link_options(${target} PRIVATE -T${LDS_OUT})

  if(ARG_FLASH)
    lds_eval(_flash_start "LD_BASE(${ARG_REGION})")
    target_enable_st_flash(${target} ${_flash_start})
  endif()

endfunction()
