#include "data_flow_graph.hpp"
#include <imnodes.h>

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

    void DataFlowGraph::drawLinks()
    {
        for (int i = 0; i < connections_.size(); ++i)
        {
            const std::pair<int, int> p = connections_[i];
            imnodes::Link(i, p.first, p.second);
        }
    }

    void DataFlowGraph::addPendingConnections()
    {
        int start_attr, end_attr;
        if (imnodes::IsLinkCreated(&start_attr, &end_attr))
        {
            connections_.emplace_back(std::make_pair(start_attr, end_attr));
        }
    }

    void DataFlowGraph::deletePendingConnections()
    {
        int link_id;
        if (imnodes::IsLinkDestroyed(&link_id))
        {
            connections_.erase(connections_.begin() + link_id);
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
