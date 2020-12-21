#pragma once
#include <string>
#include <imgui.h>
namespace gui
{
    class BaseModal
    {
    public:
        enum class Position
        {
            none,
            center
        };

    public:
        explicit BaseModal(const std::string &name, int window_flags = ImGuiWindowFlags_AlwaysAutoResize, const Position position = Position::center);
        void open();
        void draw();
        const std::string &name() const;
        virtual ~BaseModal() = default;

    protected:
        virtual void drawContent() = 0;

    protected:
        const std::string name_;
        bool want_to_open_;
        bool is_open_;
        ImGuiWindowFlags win_flags_;
        Position position_;
    };
} // namespace gui
