cmake_minimum_required(VERSION 3.14)

add_executable(nmea 
    src/parser.cpp
)
target_include_directories(nmea PUBLIC include)
set_property(TARGET nmea PROPERTY CXX_STANDARD 20)
target_link_libraries(nmea PUBLIC 
    Boost::headers
)