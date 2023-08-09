if(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/3rd/windows/include)
    link_directories(${CMAKE_SOURCE_DIR}/3rd/windows/bin)
    link_directories(${CMAKE_SOURCE_DIR}/3rd/windows/lib)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D__STDC_CONSTANT_MACROS")

    set(PLAYER_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_clock.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_clock.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_decoder.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_decoder.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_def.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_errors.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_filter.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_filter.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame_queue.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame_queue.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_internal.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_internal.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_packet_queue.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_packet_queue.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_pod.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_sdl.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_sdl.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_utils.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_utils.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_video_state.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_video_state.h)

    add_library(player SHARED ${CMAKE_CURRENT_SOURCE_DIR}/core/player.c)
    target_sources(player PRIVATE ${PLAYER_SRC})
    target_link_libraries(player PRIVATE avdevice avfilter avformat  avcodec swresample swscale avutil sdl2)

    add_executable(test_player ${CMAKE_CURRENT_SOURCE_DIR}/core/test_player.cpp)
    target_link_libraries(test_player PRIVATE player)

    add_subdirectory(qt_ui)
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

    set(PLAYER_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_clock.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_clock.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_decoder.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_decoder.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_def.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_errors.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_filter.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_filter.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame_queue.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_frame_queue.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_internal.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_internal.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_packet_queue.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_packet_queue.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_pod.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_sdl.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_sdl.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_utils.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_utils.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_video_state.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_video_state.h)


    add_library(player SHARED ${CMAKE_CURRENT_SOURCE_DIR}/core/player.c)
    target_sources(player PRIVATE ${PLAYER_SRC})
    target_link_libraries(player PRIVATE ${SWRESAMPLE_LIBRARIES} ${SWSCALE_LIBRARIES} ${AVDEVICE_LIBRARIES} ${AVFILTER_LIBRARIES} ${FFMPEG_LIBRARIES} ${AVUTIL_LIBRARIES}  SDL2::SDL2  m pthread)
    target_include_directories(player PRIVATE ${FFMPEG_INCLUDE_DIRS}  ${AVFILTER_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
endif(WIN32)

