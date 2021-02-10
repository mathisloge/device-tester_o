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

    void BaseSlot::id(const int id)
    {
        id_ = id;
    }

    int BaseSlot::id() const
    {
        return id_;
    }

    void BaseSlot::draw()
    {
        ImGui::Text(name_.c_str());
    }

} // namespace gui::df
