cmake_minimum_required(VERSION 3.14)
set(CONNECTION_SRC 
    src/connection/connection_handle.cpp
    src/connection/connection.cpp
    src/connection/serial_connection.cpp
)

set(PROTOCOL_SRC 
    src/protocols/protocol.cpp
    src/protocols/protocol_ublox.cpp
    src/protocols/ublox_detail.cpp
    src/protocols/protocol_nmea.cpp
)

set(GNSS_SRC 
    src/gnss/gnss_device.cpp
    src/gnss/ublox_device.cpp
)


set(GUI_SRC 

)

add_library(gps-tester STATIC
    ${CONNECTION_SRC}
    ${PROTOCOL_SRC}
    ${GNSS_SRC}
    ${GUI_SRC}
)
set_property(TARGET gps-tester PROPERTY CXX_STANDARD 20)
if(MSVC)
    set_property(TARGET gps-tester APPEND PROPERTY COMPILE_OPTIONS "/bigobj")
    set_property(TARGET gps-tester APPEND PROPERTY COMPILE_OPTIONS "/Zm10")
endif()

set(COMPILE_DEFS BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
if(WIN32)
    # fix to win 10 yet
    list(APPEND COMPILE_DEFS _WIN32_WINNT=0x0A00)
endif()
target_compile_definitions(gps-tester PUBLIC ${COMPILE_DEFS})
target_link_libraries(gps-tester PUBLIC 
    Boost::headers
    spdlog::spdlog
    cc::comms
    cc::ublox
    nmea-parser
)

add_executable(gps-tester-app 
    src/main.cpp
)
set_property(TARGET gps-tester-app PROPERTY CXX_STANDARD 20)
target_link_libraries(gps-tester-app PRIVATE gps-tester)

if(BUILD_TESTING)
    add_executable(gps-tester-tests 
        test/main.cpp
        test/protocols/protocol_dispatcher.cpp
        test/protocols/protocol_ublox.cpp
    )
    set_property(TARGET gps-tester-tests PROPERTY CXX_STANDARD 20)
    target_link_libraries(gps-tester-tests Catch2::Catch2 gps-tester)
    target_include_directories(gps-tester-tests PRIVATE src)

    include(Catch)
    catch_discover_tests(gps-tester-tests)
endif()

install(TARGETS gps-tester-app)
