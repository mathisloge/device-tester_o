#include "slot.hpp"
#include <imgui.h>
#include <imnodes.h>
namespace gui::df
{
    Slot::Slot(const std::string &name, const std::string &datatype)
        : id_{slot_id_counter++},
          name_{name},
          datatype_{datatype}
    {
    }

    int Slot::id() const
    {
        return id_;
    }
    
    bool Slot::canConnectWith(const Slot &rhs) const
    {
        return datatype_ == rhs.datatype_;
    }

    void Slot::drawSlot()
    {
        begin();
        drawContent();
        end();
    }

    void Slot::drawContent()
    {
        ImGui::Text(name_.c_str());
    }

    //========= INPUT SLOT =========
    InputSlot::InputSlot(const std::string &name, const std::string &datatype)
        : Slot{name, datatype}
    {
    }
    void InputSlot::begin()
    {
        imnodes::BeginInputAttribute(id_);
    }
    void InputSlot::end()
    {
        imnodes::EndInputAttribute();
    }

    //========= OUTPUT SLOT =========
    OutputSlot::OutputSlot(const std::string &name, const std::string &datatype)
        : Slot{name, datatype}
    {
    }
    void OutputSlot::begin()
    {
        imnodes::BeginOutputAttribute(id_);
    }
    void OutputSlot::end()
    {
        imnodes::EndOutputAttribute();
    }
} // namespace gui::df
