cmake_minimum_required(VERSION 3.14)

find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
add_library(ImGui::ImGui ALIAS imgui::imgui)
find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED GL)
find_package(MagnumIntegration REQUIRED ImGui)

if(CORRADE_TARGET_ANDROID)
    find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED Sdl2Application)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(CONNECTION_SRC 
    src/connection/connection_handle.cpp
    src/connection/connection.cpp
    src/connection/serial_connection.cpp
    src/connection/tcp_connection.cpp
    #src/connection/udp_connection.hpp
)

set(PROTOCOL_SRC 
    src/protocols/protocol.cpp
    src/protocols/protocol_ublox.cpp
    src/protocols/ublox_detail.cpp
    src/protocols/protocol_nmea.cpp
)

set(DEVICE_SRC 
    src/devices/device_connection.cpp
    src/devices/base_device.cpp
    src/devices/ublox_device.cpp
)

set(CONFIG_SRC
    src/config/config_manager.cpp
)

set(GUI_SRC 
    src/gui/imgui_commons.cpp
    src/gui/device_manager.cpp
    src/gui/app.cpp
    src/gui/widgets/raw_text_output.cpp
    src/gui/widgets/raw_mem_viewer.cpp
    src/gui/widgets/plot_throughput.cpp
    src/gui/modals/base_modal.cpp
    src/gui/modals/device_create.cpp
    src/gui/windows/base_window.cpp
    src/gui/windows/connection_win.cpp
    src/gui/windows/serial_connection_win.cpp
    src/gui/windows/tcp_connection_win.cpp
)

add_library(gps-tester STATIC
    ${CONNECTION_SRC}
    ${PROTOCOL_SRC}
    ${DEVICE_SRC}
    ${CONFIG_SRC}
    ${GUI_SRC}
)
set_property(TARGET gps-tester PROPERTY CXX_STANDARD 20)
if(MSVC)
    set_property(TARGET gps-tester APPEND PROPERTY COMPILE_OPTIONS "/bigobj")
endif()

set(COMPILE_DEFS BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
if(WIN32)
    # fix to win 10 yet
    list(APPEND COMPILE_DEFS _WIN32_WINNT=0x0A00)
endif()
target_compile_definitions(gps-tester PUBLIC ${COMPILE_DEFS})
target_link_libraries(gps-tester PUBLIC 
    Boost::headers
    spdlog::spdlog
    cc::comms
    cc::ublox
    nmea::nmea
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    imgui::imgui
    implot::implot
    imgui::club
    MagnumIntegration::ImGui
)

add_executable(gps-tester-app 
    src/main.cpp
)
set_property(TARGET gps-tester-app PROPERTY CXX_STANDARD 20)
target_link_libraries(gps-tester-app PRIVATE imgui::imgui gps-tester)

# add hidpi instructions
if(WIN32)
    if(MSVC)
        target_sources(gps-tester-app PRIVATE WindowsHiDPI.manifest)
    elseif(MINGW)
        target_sources(gps-tester-app PRIVATE WindowsHiDPI.rc)
    endif()
endif()

add_custom_command(TARGET gps-tester-app PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory fonts/ $<TARGET_FILE_DIR:gps-tester-app>/fonts)

if(BUILD_TESTING)
    add_executable(gps-tester-tests 
        test/main.cpp
        test/protocols/protocol_dispatcher.cpp
        test/protocols/protocol_ublox.cpp
    )
    set_property(TARGET gps-tester-tests PROPERTY CXX_STANDARD 20)
    target_link_libraries(gps-tester-tests Catch2::Catch2 gps-tester)
    target_include_directories(gps-tester-tests PRIVATE src)

    include(Catch)
    catch_discover_tests(gps-tester-tests)
endif()

install(TARGETS gps-tester-app)
