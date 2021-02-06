project(PluginUbloxProtocol)

find_package(LibComms CONFIG REQUIRED)
find_package(ublox CONFIG REQUIRED)

set(SOURCES         
    src/protocol_ublox.cpp
    src/ublox_detail.cpp
    src/protocol_plugin.cpp
)
corrade_add_plugin(ProtocolUblox 
    "${PROTOCOL_INSTALL_DBG_DIR}"
    "${PROTOCOL_INSTALL_REL_DIR}"
    ProtocolUblox.conf 
    ${SOURCES}
)
set_property(TARGET ProtocolUblox PROPERTY CXX_STANDARD 20)
if(MSVC)
    set_property(TARGET ProtocolUblox APPEND PROPERTY COMPILE_OPTIONS "/bigobj")
endif()

target_include_directories(
    ProtocolUblox PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
target_link_libraries(ProtocolUblox PRIVATE 
    cc::comms
    cc::ublox
    plugin::protocol
)
