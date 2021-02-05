project(ProtocolPluginUblox)

find_package(LibComms CONFIG REQUIRED)
find_package(ublox CONFIG REQUIRED)


add_library(ProtocolPluginUblox  SHARED
    src/protocol_ublox.cpp
    src/ublox_detail.cpp
    src/protocol_plugin.cpp
)

set_property(TARGET ProtocolPluginUblox PROPERTY CXX_STANDARD 20)
if(MSVC)
    set_property(TARGET ProtocolPluginUblox APPEND PROPERTY COMPILE_OPTIONS "/bigobj")
endif()

target_include_directories(
    ProtocolPluginUblox PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(ProtocolPluginUblox PRIVATE 
    cc::comms
    cc::ublox
    plugin::protocol
)
