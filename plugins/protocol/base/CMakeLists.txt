project(ProtocolPlugin)

find_package(Corrade REQUIRED PluginManager)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_library(ProtocolPlugin INTERFACE)
add_library(plugin::protocol ALIAS ProtocolPlugin)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX include)

target_link_libraries(ProtocolPlugin INTERFACE Corrade::PluginManager)

target_include_directories(
    ProtocolPlugin INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
)
