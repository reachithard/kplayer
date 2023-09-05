if(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/3rd/windows/include)
    include_directories(${CMAKE_SOURCE_DIR}/3rd/vendor/whisper.cpp-1.4.2)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/qt_ui)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/asr)
    link_directories(${CMAKE_SOURCE_DIR}/3rd/windows/bin)
    link_directories(${CMAKE_SOURCE_DIR}/3rd/windows/lib)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D__STDC_CONSTANT_MACROS")

    set(ASR_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/asr/asr_whisper_wrapper.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/asr/asr_whisper_wrapper.h)
    add_library(asr SHARED ${CMAKE_CURRENT_SOURCE_DIR}/asr/asr_whisper_wrapper.cpp)
    target_sources(asr PRIVATE ${ASR_SRC})
    target_link_libraries(asr PRIVATE whisper)

    add_executable(test_asr ${CMAKE_CURRENT_SOURCE_DIR}/asr/test_asr_whisper_wrapper.cpp)
    target_link_libraries(test_asr PRIVATE asr)

    set(PLAYER_SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_asr.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_asr.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_asr_queue.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_asr_queue.h
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
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_soundtouch.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_soundtouch.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_utils.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_utils.h
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_video_state.c
            ${CMAKE_CURRENT_SOURCE_DIR}/core/player_video_state.h)

    add_library(player SHARED ${CMAKE_CURRENT_SOURCE_DIR}/core/player.c)
    target_sources(player PRIVATE ${PLAYER_SRC})
    target_link_libraries(player PRIVATE avdevice avfilter avformat  avcodec swresample swscale avutil sdl2 SoundTouchDll_x64 asr)

    add_executable(test_player ${CMAKE_CURRENT_SOURCE_DIR}/core/test_player.cpp)
    target_link_libraries(test_player PRIVATE player)

    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    set(PROJECT_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_main.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_main.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_media_list.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_media_list.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_about.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_about.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_about.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_ctrl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_ctrl.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_ctrl.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_global_init.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_global_init.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_setting.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_setting.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_setting.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_show.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_show.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_show.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_slider.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_slider.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_title.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_title.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_title.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_play_list.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_play_list.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_play_list.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_wrapper.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qt_ui/kplayer_wrapper.h
            )

    set(RCC_FILES ${CMAKE_SOURCE_DIR}/res/player_res.qrc)
    add_executable(kplayer
            ${PROJECT_SOURCES} ${RCC_FILES}
            )


    target_link_libraries(kplayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets player intf )
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

