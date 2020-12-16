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


target_link_libraries(gps-tester-app PRIVATE 
    cc::comms
    cc::ublox
    Boost::headers
)
set_property(TARGET gps-tester-app PROPERTY CXX_STANDARD 20)

install(TARGETS gps-tester-app)