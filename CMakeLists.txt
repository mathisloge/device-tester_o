cmake_minimum_required(VERSION 3.14)
project(DeviceTester
        VERSION 0.1.0
        DESCRIPTION "device tester application"
        HOMEPAGE_URL https://github.com/mathisloge/device-tester
        LANGUAGES CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(spdlog CONFIG REQUIRED)
find_package(ublox CONFIG REQUIRED)
find_package(LibComms CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

add_library(imgui-club INTERFACE)
target_include_directories(imgui-club INTERFACE imgui_club/imgui_memory_editor)
add_library(imgui::club ALIAS imgui-club)

option(ENABLE_TESTS ON)

set(ENABLE_TESTS ON)
if(ENABLE_TESTS)
    include(CTest)
endif()

if(BUILD_TESTING)
    find_package(Catch2 REQUIRED)
endif()

add_subdirectory(nmea)
add_subdirectory(plugins)
add_subdirectory(app)

add_subdirectory(docs)

# Package Generator  #######################################################
set(CPACK_PACKAGE_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "mathisloge")
set(CPACK_PACKAGE_CONTACT "https://github.com/mathisloge")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
if(WIN32)
    set(CPACK_GENERATOR "WIX")
endif()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Mathis Logemann")
include (CPack)
