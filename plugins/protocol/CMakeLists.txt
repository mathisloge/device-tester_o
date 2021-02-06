find_package(Corrade REQUIRED PluginManager)

set(PROTOCOL_INSTALL_DBG_DIR "${CMAKE_INSTALL_PREFIX}/bin/protocols/dbg")
set(PROTOCOL_INSTALL_REL_DIR "${CMAKE_INSTALL_PREFIX}/bin/protocols/rel")

add_subdirectory(base)
add_subdirectory(ublox)
add_subdirectory(nmea)

install(TARGETS ProtocolPlugin
    EXPORT ProtocolPlugin-targets
    COMPONENT ProtocolPlugin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin/protocols
    INCLUDES DESTINATION include
)
