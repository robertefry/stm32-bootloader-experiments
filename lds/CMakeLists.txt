
cmake_minimum_required(VERSION 3.12)

add_library(lds INTERFACE)

target_include_directories(lds
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
