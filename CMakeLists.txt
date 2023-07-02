cmake_minimum_required(VERSION 3.12)

project(kplayer)
set(CMAKE_CXX_STANDARD 14)
set(C_STANDARD 99)


SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE} version")

# # 设置基础变量
SET(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/output)

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

    include(CTest)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
    message(STATUS "test")

    add_subdirectory(test)
endif(DOWNLOAD_THIRDPARTY)