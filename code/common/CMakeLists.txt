
cmake_minimum_required(VERSION 3.24)

add_library(common STATIC
  src/memorymap.c
  src/syscalls.c
  src/systick.c
  src/ringview.c
  src/uart.c
)
target_set_standards(common)

target_link_options(common PUBLIC
  -nostartfiles
)

target_link_libraries(common INTERFACE
  "$<LINK_GROUP:RESCAN,-lc,$<TARGET_FILE:common>>"
)

target_link_libraries(common PUBLIC
  stm32f4
  libopencm3_stm32f4
)

target_include_directories(common
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
