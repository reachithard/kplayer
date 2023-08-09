set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        kplayer_main.cpp
        kplayer_main.h
        kplayer_main.ui
        )

add_executable(kplayer
        ${PROJECT_SOURCES}
        )

target_link_libraries(kplayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets player intf)