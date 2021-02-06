#include "slot.hpp"
#include <imgui.h>
#include <imnodes.h>
namespace gui::df
{
    Slot::Slot(const std::string &name, const Type type)
        : id_{slot_id_counter++},
          name_{name},
          type_{type}
    {
    }

    void Slot::drawSlot()
    {
        switch (type_)
        {
        case Type::input:
            imnodes::BeginInputAttribute(id_);
            break;
        case Type::output:
            imnodes::BeginOutputAttribute(id_);
            break;
        }

        drawContent();

        switch (type_)
        {
        case Type::input:
            imnodes::EndInputAttribute();
            break;
        case Type::output:
            imnodes::EndOutputAttribute();
            break;
        }
    }

    void Slot::drawContent()
    {
        ImGui::Text(name_.c_str());
    }
} // namespace gui::df
