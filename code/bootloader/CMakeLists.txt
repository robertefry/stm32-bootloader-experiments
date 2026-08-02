
cmake_minimum_required(VERSION 3.12)

add_executable(bootloader
  src/main.c
)
target_set_standards(bootloader)

target_link_script(bootloader FLASH REGION BOOTLOADER)

target_link_libraries(bootloader PUBLIC
  common
  stm32f4
  libopencm3_stm32f4
)
