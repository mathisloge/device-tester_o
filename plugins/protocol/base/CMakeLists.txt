project(ProtocolPlugin)

add_library(ProtocolPlugin INTERFACE include/dt-protocol/protocol.hpp include/dt-protocol/protocol_types.hpp)
add_library(plugin::protocol ALIAS ProtocolPlugin)

set_property(TARGET ProtocolPlugin PROPERTY CXX_STANDARD 20)

target_include_directories(
    ProtocolPlugin INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
)
