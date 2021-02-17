#pragma once
#include "node_types.hpp"
#include "dataflow_export.h"
#include <nlohmann/json.hpp>

namespace dt::df
{
    class DATAFLOW_EXPORT BaseNode
    {
    public:
        BaseNode(const NodeId id,
                 const NodeKey &key,
                 const std::string &title,
                 Slots &&inputs,
                 Slots &&outputs);
        BaseNode(const nlohmann::json &, Slots &&inputs, Slots &&outputs);

        NodeId id() const;
        const NodeKey &key() const;
        virtual void render();
        void setPosition(int x, int y, bool is_screen_coords = false);

        const Slots &inputs() const;
        const Slots &outputs() const;
        SlotPtr inputs(const SlotId id) const;
        SlotPtr outputs(const SlotId id) const;
        SlotPtr inputByLocalId(const SlotId id) const;
        SlotPtr outputByLocalId(const SlotId id) const;

        virtual void to_json(nlohmann::json &j) const;

        virtual ~BaseNode();
        BaseNode(const BaseNode &) = delete;
        BaseNode &operator=(const BaseNode &) = delete;

    protected:
        virtual void renderCustomContent();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
