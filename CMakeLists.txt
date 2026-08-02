
cmake_minimum_required(VERSION 3.12)

include("cmake/get_cpm.cmake")
include("cmake/standards.cmake")
include("cmake/common.cmake")
include("cmake/st-flash.cmake")

project("BareMetal Tutorial")

add_subdirectory(lds)
add_subdirectory(code/common)
add_subdirectory(code/bootloader)
add_subdirectory(code/firmware)
