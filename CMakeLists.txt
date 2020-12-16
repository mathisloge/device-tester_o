cmake_minimum_required(VERSION 3.14)
project(gps-tester
        VERSION 0.1.0
        DESCRIPTION "gps tester application"
        HOMEPAGE_URL https://github.com/mathisloge/gps-tester
        LANGUAGES CXX)

find_package(ublox CONFIG REQUIRED)
find_package(LibComms CONFIG REQUIRED)
add_subdirectory(app)


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
