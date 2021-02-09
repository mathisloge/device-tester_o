#include "slot.hpp"
#include <imgui.h>
#include <imnodes.h>
namespace gui::df
{

    BaseSlot::BaseSlot(const std::string &name)
        : name_{name}
    {
    }
    void BaseSlot::operator()()
    {
    }

    void BaseSlot::vertexDesc(const int vertex_desc)
    {
        vertex_desc_ = vertex_desc;
    }

    int BaseSlot::vertexDesc() const
    {
        return vertex_desc_;
    }

    void BaseSlot::draw()
    {
        ImGui::Text(name_.c_str());
    }

} // namespace gui::df
