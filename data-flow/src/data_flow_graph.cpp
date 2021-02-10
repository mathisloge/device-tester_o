#include "data-flow/data_flow_graph.hpp"
#include "graph_impl.hpp"
namespace dt::df
{
    DataFlowGraph::DataFlowGraph()
        : impl_{new GraphImpl()}
    {
    }

    void DataFlowGraph::registerNode(const NodeKey &key, NodeFactory &&factory)
    {
    }
    void DataFlowGraph::addNode(const NodeKey &key)
    {
    }
    void DataFlowGraph::removeNode(const NodeId id)
    {
    }
    void DataFlowGraph::addEdge(const NodeId from, const NodeId to)
    {
        try
        {
            auto from_vert = impl_->findVertexById(from);
            auto to_vert = impl_->findVertexById(to);
            addEdge(from_vert, to_vert);
        }
        catch (const std::out_of_range &)
        {
            //! \todo we need to log the error
        }
    }
    void DataFlowGraph::addEdge(const VertexDesc from, const VertexDesc to)
    {
        impl_->addEdge(from, to);
    }
    void DataFlowGraph::removeEdge(const EdgeId id)
    {
    }

    DataFlowGraph::~DataFlowGraph()
    {
        delete impl_;
    }
} // namespace dt::df
