add_subdirectory(base)
add_subdirectory(ublox)
add_subdirectory(nmea)


install(TARGETS 
        ProtocolPlugin
        ProtocolPluginUblox
        ProtocolPluginNmea
    EXPORT ProtocolPlugin-targets
    COMPONENT ProtocolPlugin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin/protocols
    INCLUDES DESTINATION include
)
