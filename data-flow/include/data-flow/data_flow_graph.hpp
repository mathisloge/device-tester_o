#pragma once
#include <functional>
#include <boost/noncopyable.hpp>
#include "nodes/base_node.hpp"
#include "types.hpp"
namespace dt::df
{
    class GraphImpl;
    class DataFlowGraph : public boost::noncopyable
    {
    public:
        DataFlowGraph();
        void registerNode(const NodeKey &key, NodeFactory &&factory);
        void addNode(const NodeKey &key);
        void removeNode(const NodeId id);
        void addEdge(const NodeId from, const NodeId to);
        void addEdge(const VertexDesc from, const VertexDesc to);
        void removeEdge(const EdgeId id);
        virtual ~DataFlowGraph();

    private:
        GraphImpl *impl_;
        friend GraphImpl;
    };
} // namespace dt::df
