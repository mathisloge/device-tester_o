#pragma once
#include <map>
#include <string>
#include <functional>
#include "node.hpp"
namespace gui::df
{
    class DataFlowGraph final
    {
    public:
        using NodePtr = std::shared_ptr<Node>;
        using NodeFactoryFnc = std::function<NodePtr()>;

    public:
        DataFlowGraph();
        void addNode(const std::string &key);
        void registerNodeFactory(const std::string &key, NodeFactoryFnc &&factory);
        void drawNodes();
        const std::vector<NodePtr> &nodes() const;
        ~DataFlowGraph();

    private:
        std::map<std::string, NodeFactoryFnc> node_factories_;
        std::vector<NodePtr> nodes_;
    };
} // namespace gui::df
