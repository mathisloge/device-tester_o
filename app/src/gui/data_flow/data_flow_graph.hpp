#pragma once
#include <map>
#include <vector>
#include <atomic>
#include <string>
#include <functional>
#include <boost/graph/adjacency_list.hpp>
#include "data_flow_types.hpp"
#include "ui_node.hpp"
namespace gui::df
{
   
    class DataFlowGraph final
    {
    public:
       

        using NodePtr = std::shared_ptr<UiNode>;
        using NodeFactoryFnc = std::function<NodePtr(int)>;
        using NodeContainer = std::map<int, NodePtr>;

    public:
        DataFlowGraph();
        void addNode(const std::string &key);
        void registerNodeFactory(const std::string &key, NodeFactoryFnc &&factory);
        void drawNodes();
        void drawLinks();
        void addPendingConnections();
        void deletePendingConnections();
        ~DataFlowGraph();

    private:
        std::map<std::string, NodeFactoryFnc> node_factories_;
        NodeContainer nodes_;

        Graph graph_;

        std::atomic_int link_id_counter_;
    };
} // namespace gui::df
