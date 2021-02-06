project(PluginNmeaProtocol)


set(SOURCES 
    src/handler.cpp
    src/protocol_nmea.cpp
    src/protocol_plugin.cpp
)
corrade_add_plugin(ProtocolNmea 
    "${PROTOCOL_INSTALL_DBG_DIR}"
    "${PROTOCOL_INSTALL_REL_DIR}"
    ProtocolNmea.conf 
    ${SOURCES}
)
set_property(TARGET ProtocolNmea PROPERTY CXX_STANDARD 20)
target_include_directories(
    ProtocolNmea PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
target_link_libraries(ProtocolNmea PRIVATE 
    nmea::nmea
    plugin::protocol
)
