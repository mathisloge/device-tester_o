#include "data-flow/nodes/core/base_node.hpp"
#include <imgui.h>
#include <imnodes.h>
#include "json.hpp"

using json = nlohmann::json;
namespace dt::df
{

    class BaseNode::Impl
    {
    public:
        Impl(const NodeId id,
             const NodeKey &key,
             const std::string &title,
             Slots &&inputs,
             Slots &&outputs)
            : id_{id},
              key_{key},
              title_{title},
              inputs_{std::move(inputs)},
              outputs_{std::move(outputs)},
              position_was_updated_{false}
        {
        }

        void to_json(nlohmann::json &j) const
        {
            j["id"] = id_;
            j["key"] = key_;
            j["title"] = title_;
            json input_slots = json::array();
            json output_slots = json::array();
            for (const auto &slot : inputs_)
            {
                json slot_val;
                slot->to_json(slot_val);
                input_slots.emplace_back(std::move(slot_val));
            }
            for (const auto &slot : outputs_)
            {
                json slot_val;
                slot->to_json(slot_val);
                output_slots.emplace_back(std::move(slot_val));
            }
            j["inputs"] = std::move(input_slots);
            j["outputs"] = std::move(output_slots);

            const auto position = imnodes::GetNodeEditorSpacePos(id_);
            j["position"] = {{"x", position.x}, {"y", position.y}};
        }

        void setPosition(int x, int y, bool is_screen_coords = false)
        {
            position_ = {static_cast<float>(x), static_cast<float>(y)};
            is_screen_coords_ = is_screen_coords;
            position_was_updated_ = true;
        }

        void updatePosIfNeeded()
        {
            if (position_was_updated_)
            {
                if (is_screen_coords_)
                    imnodes::SetNodeEditorSpacePos(id_, position_);
                else
                    imnodes::SetNodeScreenSpacePos(id_, position_);
                position_was_updated_ = false;
            }
        }

        template <class Predicate>
        static SlotPtr find(const Slots &slots, Predicate pred)
        {
            auto slot_it = std::find_if(slots.begin(), slots.end(), pred);

            if (slot_it == slots.end())
                return nullptr;
            return *slot_it;
        }

    private:
        const NodeId id_;
        const NodeKey key_;
        const std::string title_;
        const Slots inputs_;
        const Slots outputs_;

        bool position_was_updated_;
        bool is_screen_coords_;
        ImVec2 position_;

        friend BaseNode;
    };

    BaseNode::BaseNode(const NodeId id,
                       const NodeKey &key,
                       const std::string &title,
                       Slots &&inputs,
                       Slots &&outputs)
        : impl_{new BaseNode::Impl{id, key, title, std::forward<Slots>(inputs), std::forward<Slots>(outputs)}}
    {
    }

    BaseNode::BaseNode(const nlohmann::json &json, Slots &&inputs, Slots &&outputs)
    {
        // i know, this is UB. But since the constructor shouldn't throw anything, we need to make sure, that these keys exist before the construction. aka. in GraphImpl
        impl_ = new BaseNode::Impl{json["id"], json["key"], json["title"], std::forward<Slots>(inputs), std::forward<Slots>(outputs)};
        try
        {
            const auto &pos = json.at("position");
            impl_->setPosition(pos.at("x"), pos.at("y"));
        }
        catch (...)
        {
        }
    }

    const NodeKey &BaseNode::key() const
    {
        return impl_->key_;
    }

    void BaseNode::render()
    {
        imnodes::BeginNode(impl_->id_);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(impl_->title_.c_str());
        imnodes::EndNodeTitleBar();

        for (auto &slot : impl_->inputs_)
        {
            imnodes::BeginInputAttribute(slot->id());
            slot->render();
            imnodes::EndInputAttribute();
        }

        renderCustomContent();

        for (auto &slot : impl_->outputs_)
        {
            imnodes::BeginOutputAttribute(slot->id());
            slot->render();
            imnodes::EndOutputAttribute();
        }
        imnodes::EndNode();

        impl_->updatePosIfNeeded();
    }

    void BaseNode::setPosition(int x, int y, bool is_screen_coords)
    {
        impl_->setPosition(x, y, is_screen_coords);
    }

    void BaseNode::renderCustomContent()
    {
    }

    NodeId BaseNode::id() const
    {
        return impl_->id_;
    }

    const Slots &BaseNode::inputs() const
    {
        return impl_->inputs_;
    }
    const Slots &BaseNode::outputs() const
    {
        return impl_->outputs_;
    }

    SlotPtr BaseNode::inputs(const SlotId id) const
    {
        return impl_->find(impl_->inputs_, [id](const auto &slot) {
            return slot->id() == id;
        });
    }
    SlotPtr BaseNode::outputs(const SlotId id) const
    {
        return impl_->find(impl_->outputs_, [id](const auto &slot) {
            return slot->id() == id;
        });
    }

    SlotPtr BaseNode::inputByLocalId(const SlotId id) const
    {
        return impl_->find(impl_->inputs_, [id](const auto &slot) {
            return slot->localId() == id;
        });
    }
    SlotPtr BaseNode::outputByLocalId(const SlotId id) const
    {
        return impl_->find(impl_->outputs_, [id](const auto &slot) {
            return slot->localId() == id;
        });
    }

    void BaseNode::to_json(nlohmann::json &j) const
    {
        impl_->to_json(j);
    }

    BaseNode::~BaseNode()
    {
        delete impl_;
    }

} // namespace dt::df
