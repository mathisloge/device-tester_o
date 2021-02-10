project(DataFlow)

find_package(Boost REQUIRED COMPONENTS graph)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_library(DataFlow SHARED
    src/data_flow_graph.cpp
    src/graph_impl.cpp
    src/types.cpp
    
    src/slots/base_slot.cpp
    src/slots/int_slot.cpp
    src/slots/trigger_slot.cpp

    src/nodes/base_node.cpp
    src/nodes/timer_node.cpp
)
add_library(devicetester::dataflow ALIAS DataFlow)

target_link_libraries(DataFlow PUBLIC 
    Boost::headers
    Boost::graph
)

set_property(TARGET DataFlow PROPERTY CXX_STANDARD 20)

target_include_directories(
    DataFlow PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>
)
