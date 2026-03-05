
CPMAddPackage(
  NAME libopencm3
  GITHUB_REPOSITORY libopencm3/libopencm3
  GIT_TAG 7ab938990c8bd232f1cd461129560ab49a880109
  DOWNLOAD_ONLY YES
)

execute_process(
  COMMAND make -C ${libopencm3_SOURCE_DIR}
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${libopencm3_SOURCE_DIR}
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "Failed to build libopencm3.")
endif()

file(GLOB libopencm3_ARCHIVES
  ${libopencm3_SOURCE_DIR}/lib/libopencm3_*.a
)

foreach(archive ${libopencm3_ARCHIVES})

  get_filename_component(libopencm3_archive ${archive} NAME_WLE)

  add_library(${libopencm3_archive} STATIC IMPORTED GLOBAL)

  set_target_properties(${libopencm3_archive} PROPERTIES
    IMPORTED_LOCATION ${archive}
    INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include
  )

  add_dependencies(${libopencm3_archive} libopencm3_build)

endforeach()
