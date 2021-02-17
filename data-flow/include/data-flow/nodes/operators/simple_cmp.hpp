#pragma once
#include "../core/base_node.hpp"

namespace dt::df::operators
{
    class DATAFLOW_EXPORT SimpleCmp : public BaseNode
    {
    public:
        SimpleCmp(const NodeId id,
                  const NodeKey &key,
                  const std::string &title,
                  const SlotId in_a, const std::string &in_a_name,
                  const SlotId in_b, const std::string &in_b_name,
                  const SlotId out_res, const std::string &result_name,
                  const SlotId true_res,
                  const SlotId false_res);
        SimpleCmp(const nlohmann::json &json);
        virtual ~SimpleCmp();

    protected:
        virtual bool cmp(const double a, const double b) const = 0;

    private:
        void initSlots();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df::operators
