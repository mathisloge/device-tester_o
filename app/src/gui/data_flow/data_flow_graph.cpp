#include "data_flow_graph.hpp"

namespace gui::df
{
    DataFlowGraph::DataFlowGraph()
    {
    }

    void DataFlowGraph::addNode(const std::string &key)
    {
        if (auto it = node_factories_.find(key); it != node_factories_.end())
        {
            nodes_.push_back(it->second());
        }
    }

    void DataFlowGraph::registerNodeFactory(const std::string &key, NodeFactoryFnc &&factory)
    {
        node_factories_.emplace(key, std::forward<NodeFactoryFnc>(factory));
    }

    void DataFlowGraph::drawNodes()
    {
        for (auto &node : nodes_)
        {
            node->drawNode();
        }
    }

    const std::vector<DataFlowGraph::NodePtr> &DataFlowGraph::nodes() const
    {
        return nodes_;
    }

    DataFlowGraph::~DataFlowGraph()
    {
    }
} // namespace gui::df
