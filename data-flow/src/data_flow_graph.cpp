#include "data-flow/data_flow_graph.hpp"
#include "graph_impl.hpp"
#include "data-flow/nodes/timer_node.hpp"
namespace dt::df
{
    DataFlowGraph::DataFlowGraph()
        : impl_{new GraphImpl()}
    {
    }

    void DataFlowGraph::registerBuildinNodes()
    {
        registerNode(TimerNode::kNodeKey, [](NodeIdGenerator &node_id, SlotIdGenerator &slot_id) {
            return std::make_shared<TimerNode>(node_id++, slot_id++, slot_id++);
        });
    }

    void DataFlowGraph::registerNode(const NodeKey &key, NodeFactory &&factory)
    {
        impl_->registerNodeFactory(key, std::forward<NodeFactory>(factory));
    }
    void DataFlowGraph::addNode(const NodeKey &key)
    {
        impl_->createNode(key);
    }
    void DataFlowGraph::removeNode(const NodeId id)
    {
        impl_->removeNode(id);
    }
    void DataFlowGraph::addEdge(const NodeId from, const NodeId to)
    {
        try
        {
            auto from_vert = impl_->findVertexById(from);
            auto to_vert = impl_->findVertexById(to);
            impl_->addEdge(from_vert, to_vert);
        }
        catch (const std::out_of_range &)
        {
            //! \todo we need to log the error
        }
    }

    void DataFlowGraph::removeEdge(const EdgeId id)
    {
        impl_->removeEdge(id);
    }

    void DataFlowGraph::render()
    {
        impl_->renderNodes();
        impl_->renderLinks();
    }

    DataFlowGraph::~DataFlowGraph()
    {
        delete impl_;
    }
} // namespace dt::df
