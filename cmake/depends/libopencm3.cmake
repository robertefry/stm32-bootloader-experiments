
CPMAddPackage(
  NAME libopencm3
  GITHUB_REPOSITORY libopencm3/libopencm3
  GIT_TAG 7ab938990c8bd232f1cd461129560ab49a880109
  DOWNLOAD_ONLY YES
)

execute_process(
  COMMAND make -C ${libopencm3_SOURCE_DIR} list-targets
  OUTPUT_VARIABLE libopencm3_targets
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
separate_arguments(libopencm3_targets)

foreach(target IN LISTS libopencm3_targets)

  string(REPLACE "/" "" target_name ${target})

  set(libopencm3_target "libopencm3_${target_name}")
  set(libopencm3_archive "${libopencm3_SOURCE_DIR}/lib/libopencm3_${target_name}.a")

  add_custom_command(
    OUTPUT ${libopencm3_archive}
    COMMAND make -C ${libopencm3_SOURCE_DIR} TARGETS=${target}
    WORKING_DIRECTORY ${libopencm3_SOURCE_DIR}
    COMMENT "Building ${libopencm3_target}"
  )

  add_custom_target(
    ${libopencm3_target}_build
    DEPENDS ${libopencm3_archive}
  )

  add_library(${libopencm3_target} STATIC IMPORTED GLOBAL)

  set_target_properties(
    ${libopencm3_target} PROPERTIES
    IMPORTED_LOCATION ${libopencm3_archive}
    INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include
  )

  add_dependencies(
    ${libopencm3_target}
    ${libopencm3_target}_build
  )

endforeach()
