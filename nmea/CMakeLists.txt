cmake_minimum_required(VERSION 3.14)

add_library(nmea-parser INTERFACE)
target_include_directories(nmea-parser INTERFACE include)
target_link_libraries(nmea-parser INTERFACE Boost::headers)


add_executable(nmea-exe-test 
    src/parser.cpp
)
set_property(TARGET nmea-exe-test PROPERTY CXX_STANDARD 20)
target_link_libraries(nmea-exe-test PUBLIC 
    nmea-parser
)