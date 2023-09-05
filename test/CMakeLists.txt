if(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/3rd/windows/include)
    include_directories(${CMAKE_SOURCE_DIR}/3rd/vendor/whisper.cpp-1.4.2)
    link_directories(${CMAKE_SOURCE_DIR}/3rd/windows/bin)
    link_directories(${CMAKE_SOURCE_DIR}/3rd/windows/lib)

    add_executable(env env.cpp)

    add_executable(test_ffmpeg_audio test_ffmpeg_audio.cpp ${CMAKE_SOURCE_DIR}/src/asr/asr_whisper_wrapper.cpp
            ${CMAKE_SOURCE_DIR}/src/asr/asr_whisper_wrapper.h)
    target_link_libraries(test_ffmpeg_audio PRIVATE whisper avdevice avfilter avformat  avcodec swresample swscale avutil sdl2)
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


