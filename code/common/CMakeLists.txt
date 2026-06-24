
cmake_minimum_required(VERSION 3.12)

add_library(common STATIC
  src/ringview.c
  src/systick.c
  src/uart.c
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
