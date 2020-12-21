#pragma once
#include <string>
namespace gui
{
    class BaseWindow
    {
    public:
        explicit constexpr BaseWindow(const std::string &name);
        void draw();
        void setOpenState(bool is_open);
        bool isOpen() const;
        virtual ~BaseWindow() = default;

    protected:
        virtual void drawContent() = 0;

    protected:
        const std::string &name_;
        bool is_open_;
    };
} // namespace gui
