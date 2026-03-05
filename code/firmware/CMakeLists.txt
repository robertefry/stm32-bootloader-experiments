
cmake_minimum_required(VERSION 3.12)

add_executable(firmware
  src/main.c
  src/systick.c
)
target_set_standards(firmware)

target_enable_st_flash(firmware 0x08008000)

target_link_options(firmware PRIVATE
  -T${CMAKE_CURRENT_LIST_DIR}/nucleo-f446re.ld
)

target_link_libraries(firmware PUBLIC
  common
  stm32f4
  libopencm3_stm32f4
)
