cmake_minimum_required(VERSION 3.12)

project(kplayer)
set(CMAKE_CXX_STANDARD 14)
set(C_STANDARD 99)
set(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic") #  relocation R_X86_64_PC32 against symbol `ff_M24A' can not be used when making a shared object; recompile with -fPIC
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE} version")

# # 设置基础变量
SET(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/output)

if(WIN32)
    INCLUDE(cmake/utils/intf.cmake)
    add_subdirectory(3rd/vendor/whisper.cpp-1.4.2)
    add_subdirectory(src)
    add_subdirectory(test)
else(WIN32)
if(DOWNLOAD_THIRDPARTY)
    add_subdirectory(3rd)
else(DOWNLOAD_THIRDPARTY)

# # 设置cmake module path路径便于find_package
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# # 链接interface 便于开发
    INCLUDE(cmake/utils/intf.cmake)
    INCLUDE(cmake/utils/gtest.cmake)
    INCLUDE(cmake/utils/spdlog.cmake)
    INCLUDE(cmake/utils/ffmpeg.cmake)
    INCLUDE(cmake/utils/sdl.cmake)

    add_subdirectory(src)

    include(CTest)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
    message(STATUS "test")

    add_subdirectory(test)
endif(DOWNLOAD_THIRDPARTY)
endif(WIN32)