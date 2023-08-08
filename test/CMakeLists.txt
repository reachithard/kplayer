if(WIN32)
    add_executable(env env.cpp)
else(WIN32)
    find_package(FFmpeg REQUIRED)
    find_package(sdl2 REQUIRED)

    include(CMakePrintHelpers)
    cmake_print_variables(FFMPEG_FOUND)
    cmake_print_variables(sdl2_FOUND)
    cmake_print_variables(AVFILTER_FOUND)
    cmake_print_variables(FFMPEG_LIBRARIES)
    cmake_print_variables(FFMPEG_INCLUDE_DIRS)
    cmake_print_variables(AVUTIL_LIBRARIES)
    cmake_print_variables(SWRESAMPLE_LIBRARIES)


    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd)
    message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")


    add_executable(ffmpeg_ffplay 3rd/ffmpeg_ffplay.c)
    target_sources(ffmpeg_ffplay PRIVATE 3rd/cmdutils.c 3rd/opt_common.c)
    target_link_libraries(ffmpeg_ffplay PRIVATE ${SWRESAMPLE_LIBRARIES} ${SWSCALE_LIBRARIES} ${AVDEVICE_LIBRARIES} ${AVFILTER_LIBRARIES} ${FFMPEG_LIBRARIES} ${AVUTIL_LIBRARIES}  SDL2::SDL2  m pthread)
    target_include_directories(ffmpeg_ffplay PRIVATE ${FFMPEG_INCLUDE_DIRS}  ${AVFILTER_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})


endif(WIN32)


