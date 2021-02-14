#pragma once
#include "../core/base_node.hpp"

namespace dt::df::operators
{
    class DATAFLOW_EXPORT SimpleOp : public BaseNode
    {
    public:
        SimpleOp(const NodeId id,
                 const NodeKey &key,
                 const std::string &title,
                 const SlotId in_a, const std::string &in_a_name,
                 const SlotId in_b, const std::string &in_b_name,
                 const SlotId out_res, const std::string &result_name = "result");
        SimpleOp(const nlohmann::json &json);
        virtual ~SimpleOp();

    protected:
        virtual double calc(const double a, const double b) const = 0;

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df::operators
