project(DataFlow)

find_package(Boost REQUIRED COMPONENTS graph)

include(GenerateExportHeader)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(DataFlow SHARED
    src/gui.cpp
    src/data_flow_graph.cpp
    src/data_flow_editor.cpp
    src/graph_impl.cpp
    src/priv_types.cpp
    
    src/slots/base_slot.cpp
    src/slots/int_slot.cpp
    src/slots/trigger_slot.cpp

    src/nodes/base_node.cpp
    src/nodes/timer_node.cpp
)
add_library(devicetester::dataflow ALIAS DataFlow)
generate_export_header(DataFlow)

target_link_libraries(DataFlow PUBLIC 
    Boost::headers
    Boost::graph
    imgui::imgui
)

target_link_libraries(DataFlow PRIVATE
    imgui::imnodes
)

set_property(TARGET DataFlow PROPERTY CXX_STANDARD 20)

target_include_directories(
    DataFlow PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)
