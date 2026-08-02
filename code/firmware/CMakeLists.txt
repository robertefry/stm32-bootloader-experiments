
cmake_minimum_required(VERSION 3.12)

add_executable(firmware
  src/main.c
)
target_set_standards(firmware)

target_link_script(firmware FLASH REGION FIRMWARE)

target_link_libraries(firmware PUBLIC
  common
  stm32f4
  libopencm3_stm32f4
)
