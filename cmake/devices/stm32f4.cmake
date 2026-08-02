
add_library(stm32f4 INTERFACE)

target_compile_definitions(stm32f4 INTERFACE
  STM32F4
)

target_compile_options(stm32f4 INTERFACE
  -mthumb -ffunction-sections -fdata-sections
  -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
)

target_link_options(stm32f4 INTERFACE
  -mthumb -Wl,--gc-sections
  -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
)
