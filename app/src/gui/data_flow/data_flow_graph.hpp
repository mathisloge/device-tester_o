#pragma once
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <boost/graph/adjacency_list.hpp>

#include "node.hpp"
namespace gui::df
{
    struct VertexInfo
    {
    };
    class DataFlowGraph final
    {
    public:
        using Graph = boost::adjacency_list<boost::vecS,
                                            boost::vecS,
                                            boost::directedS,
                                            VertexInfo, 
                                            boost::property<boost::edge_index_t, int>>;

        using NodePtr = std::shared_ptr<Node>;
        using NodeFactoryFnc = std::function<NodePtr()>;
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
        std::vector<std::pair<int, int>> connections_;

        Graph graph_;
    };
} // namespace gui::df
