#pragma once
#include <string>
#include <boost/noncopyable.hpp>
namespace gui
{
    class BaseWindow : public boost::noncopyable
    {
    public:
        explicit BaseWindow(const std::string &name);
        const std::string &name() const;
        void draw();
        bool isOpen() const;
        bool &openState();
        virtual ~BaseWindow() = default;

    protected:
        virtual void drawContent() = 0;

    protected:
        const std::string name_;
        bool is_open_;
        int flags_;
    };
} // namespace gui
