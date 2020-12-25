project(ProtocolPlugin)

add_library(ProtocolPlugin INTERFACE)
add_library(plugin::protocol ALIAS ProtocolPlugin)

target_include_directories(ProtocolPlugin INTERFACE include)
target_include_directories(
    ProtocolPlugin INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
)

install(TARGETS ProtocolPlugin EXPORT ProtocolPlugin-targets
    COMPONENT ProtocolPlugin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
