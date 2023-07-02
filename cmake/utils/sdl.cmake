get_filename_component(sdl2_ROOT ${BASE_DIR}/output ABSOLUTE)

message("sdl2_ROOT ${sdl2_ROOT}")

find_package(sdl2 REQUIRED)

include(CMakePrintHelpers)
cmake_print_variables(sdl2_FOUND)
