set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_SOURCES
        main.cpp
        kplayer_main.cpp
        kplayer_main.h
        kplayer_main.ui
        kplayer_about.cpp
        kplayer_about.h
        kplayer_about.ui
        kplayer_ctrl.cpp
        kplayer_ctrl.h
        kplayer_ctrl.ui
        kplayer_setting.cpp
        kplayer_setting.h
        kplayer_setting.ui
        kplayer_show.cpp
        kplayer_show.h
        kplayer_show.ui
        kplayer_title.cpp
        kplayer_title.h
        kplayer_title.ui
        kplayer_play_list.cpp
        kplayer_play_list.h
        kplayer_play_list.ui
        kplayer_wrapper.cpp
        kplayer_wrapper.h
        )

add_executable(kplayer
        ${PROJECT_SOURCES}
        )

target_link_libraries(kplayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets player intf)