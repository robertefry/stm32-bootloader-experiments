
cmake_minimum_required(VERSION 3.12)

add_executable(bootloader
  src/main.c
)
target_set_standards(bootloader)

target_enable_st_flash(bootloader 0x08000000)

target_link_options(bootloader PRIVATE
  -T${CMAKE_CURRENT_LIST_DIR}/nucleo-f446re.ld
)

target_link_libraries(bootloader PUBLIC
  common
  stm32f4
  libopencm3_stm32f4
)
