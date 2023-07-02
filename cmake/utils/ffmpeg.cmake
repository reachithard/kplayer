get_filename_component(ffmpeg_ROOT ${BASE_DIR}/output ABSOLUTE)

message("spdlog_ROOT ${spdlog_ROOT}")

find_package(FFmpeg REQUIRED)

include(CMakePrintHelpers)
cmake_print_variables(FFMPEG_FOUND)

