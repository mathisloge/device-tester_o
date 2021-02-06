project(ProtocolPlugin)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_library(ProtocolPlugin INTERFACE 
    include/dt-protocol/protocol.hpp 
    include/dt-protocol/protocol_types.hpp
    include/dt-protocol/protocol_plugin.hpp
)
add_library(plugin::protocol ALIAS ProtocolPlugin)

target_link_libraries(ProtocolPlugin INTERFACE Corrade::PluginManager)

set_property(TARGET ProtocolPlugin PROPERTY CXX_STANDARD 20)

target_include_directories(
    ProtocolPlugin INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
)
