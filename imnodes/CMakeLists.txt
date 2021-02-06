add_library(imgui-imnodes STATIC imnodes/imnodes.cpp)
target_include_directories(imgui-imnodes PUBLIC imnodes)
target_link_libraries(imgui-imnodes PRIVATE imgui::imgui)
add_library(imgui::imnodes ALIAS imgui-imnodes)
