project(DataFlow)

find_package(imgui CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS graph)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
include(GenerateExportHeader)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(DataFlow STATIC
    src/gui.cpp
    src/data_flow_graph.cpp
    src/data_flow_editor.cpp
    src/graph_impl.cpp
    src/node_display_tree.cpp
    src/id_generator.cpp
    src/priv_types.cpp
    
    src/slots/core/base_slot.cpp
    src/slots/core/number_slot.cpp
    src/slots/int_slot.cpp
    src/slots/floating_slot.cpp
    src/slots/trigger_slot.cpp
    src/slots/bool_slot.cpp

    src/nodes/core/base_node.cpp
    src/nodes/core/json.cpp
    src/nodes/utils/timer_node.cpp
    src/nodes/interaction/color_node.cpp
    src/nodes/operators/simple_op.cpp
    src/nodes/operators/standard_ops.cpp
    src/nodes/operators/simple_cmp.cpp
)
add_library(devicetester::dataflow ALIAS DataFlow)
generate_export_header(DataFlow)

target_link_libraries(DataFlow PUBLIC 
    Boost::headers
    Boost::graph
    imgui::imgui
    nlohmann_json::nlohmann_json
    sol2::sol2
)

target_link_libraries(DataFlow PRIVATE
    imgui::imnodes
    spdlog::spdlog
)

set_property(TARGET DataFlow PROPERTY CXX_STANDARD 20)

target_include_directories(
    DataFlow PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)
