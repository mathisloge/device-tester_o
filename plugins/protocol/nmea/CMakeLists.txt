project(ProtocolPluginNmea)

add_library(ProtocolPluginNmea  SHARED
    src/handler.cpp
    src/protocol_nmea.cpp
    src/protocol_plugin.cpp
)

set_property(TARGET ProtocolPluginNmea PROPERTY CXX_STANDARD 20)

target_include_directories(
    ProtocolPluginNmea PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(ProtocolPluginNmea PRIVATE 
    nmea::nmea
    plugin::protocol
)
