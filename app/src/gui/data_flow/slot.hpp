#pragma once
#include <string>
#include <atomic>
namespace gui::df
{
    class BaseSlot
    {
    public:
        explicit BaseSlot(const std::string &name);
        void vertexDesc(const int vertex_desc);
        int vertexDesc() const;
        virtual void operator()();
        virtual void draw();

    private:
    const std::string name_;
        int vertex_desc_;
    };
} // namespace gui::df
