#pragma once
#include <imgui.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#ifdef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/AndroidApplication.h>
#elif defined(CORRADE_TARGET_EMSCRIPTEN)
#include <Magnum/Platform/EmscriptenApplication.h>
#else
#include <Magnum/Platform/Sdl2Application.h>
#endif

#include "device_manager.hpp"
#include "modals/device_create.hpp"

namespace gui
{
    class App : public Magnum::Platform::Application
    {
    public:
        explicit App(const Arguments &arguments);

        void drawEvent() override;

        void viewportEvent(ViewportEvent &event) override;

        void keyPressEvent(KeyEvent &event) override;
        void keyReleaseEvent(KeyEvent &event) override;

        void mousePressEvent(MouseEvent &event) override;
        void mouseReleaseEvent(MouseEvent &event) override;
        void mouseMoveEvent(MouseMoveEvent &event) override;
        void mouseScrollEvent(MouseScrollEvent &event) override;
        void textInputEvent(TextInputEvent &event) override;

    private:
        void showMainMenu();
        void showDeviceMenu();

    private:
        Magnum::ImGuiIntegration::Context imgui_{Magnum::NoCreate};

        DeviceManager device_manager_;
        
        DeviceCreate device_create_modal_;
    };
} // namespace gui
