cmake_minimum_required(VERSION 3.14)
find_package(Boost REQUIRED
)
set(CONNECTION_SRC 
    src/gnss/gnss_device.cpp
    src/gnss/ublox_device.cpp
)

set(GNSS_SRC 
    src/connection/connection.cpp
    src/connection/serial_connection.cpp
)

set(GUI_SRC 

)

add_executable(gps-tester-app 
    src/main.cpp
    ${CONNECTION_SRC}
    ${GNSS_SRC}
    ${GUI_SRC}
)

set(COMPILE_DEFS BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT)
if(WIN32)
    # fix to win 10 yet
    list(APPEND COMPILE_DEFS _WIN32_WINNT=0x0A00)
endif()

target_compile_definitions(gps-tester-app PRIVATE ${COMPILE_DEFS})

target_link_libraries(gps-tester-app PRIVATE 
    cc::comms
    cc::ublox
    Boost::headers
)
set_property(TARGET gps-tester-app PROPERTY CXX_STANDARD 20)

install(TARGETS gps-tester-app)