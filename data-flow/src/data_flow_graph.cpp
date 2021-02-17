#include "data-flow/data_flow_graph.hpp"
#include "graph_impl.hpp"
#include "data-flow/nodes/utils/timer_node.hpp"
#include "data-flow/nodes/interaction/color_node.hpp"
#include "data-flow/nodes/operators/standard_ops.hpp"
namespace dt::df
{

    template <typename TSimpleOp>
    void registerSimpleOp(DataFlowGraph &fg)
    {
        fg.registerNode(
            TSimpleOp::kNodeKey,
            std::string("operators/") + TSimpleOp::kNodeName,
            [](NodeIdGenerator &node_id, SlotIdGenerator &slot_id) { return std::make_shared<TSimpleOp>(node_id(), slot_id(), slot_id(), slot_id()); },
            [](const nlohmann::json &j) { return std::make_shared<TSimpleOp>(j); });
    }

    DataFlowGraph::DataFlowGraph()
        : impl_{new GraphImpl()}
    {
    }

    void DataFlowGraph::registerBuildinNodes()
    {
        registerNode(
            TimerNode::kNodeKey,
            "utils/Timer Node",
            [](NodeIdGenerator &node_id, SlotIdGenerator &slot_id) {
                return std::make_shared<TimerNode>(node_id(), slot_id(), slot_id());
            },
            [](const nlohmann::json &j) {
                return std::make_shared<TimerNode>(j);
            });
        registerNode(
            ColorNode::kNodeKey,
            "interaction/Color Node",
            [](NodeIdGenerator &node_id, SlotIdGenerator &slot_id) {
                return std::make_shared<ColorNode>(node_id(), slot_id(), slot_id(), slot_id(), slot_id());
            },
            [](const nlohmann::json &j) {
                return std::make_shared<ColorNode>(j);
            });

        registerSimpleOp<operators::Division>(*this);
        registerSimpleOp<operators::Multiplication>(*this);
        registerSimpleOp<operators::Addition>(*this);
        registerSimpleOp<operators::Subtraction>(*this);
        registerSimpleOp<operators::Modulo>(*this);
    }

    void DataFlowGraph::registerNode(const NodeKey &key, const std::string &node_display_name, NodeFactory &&factory, NodeDeserializationFactory &&deser_factory)
    {
        impl_->registerNodeFactory(key, node_display_name, std::forward<NodeFactory>(factory), std::forward<NodeDeserializationFactory>(deser_factory));
    }
    void DataFlowGraph::addNode(const NodeKey &key, int preferred_x, int preferred_y, bool screen_space)
    {
        impl_->createNode(key, preferred_x, preferred_y, screen_space);
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

    void DataFlowGraph::save(const std::filesystem::path &file)
    {
        impl_->save(file);
    }

    void DataFlowGraph::clear()
    {
        impl_->clear();
    }

    void DataFlowGraph::clearAndLoad(const std::filesystem::path &file)
    {
        impl_->clearAndLoad(file);
    }

    void DataFlowGraph::renderNodeDisplayTree(const NodeDisplayDrawFnc &draw_fnc) const
    {
        impl_->nodeDisplayNames().drawTree(draw_fnc);
    }

    DataFlowGraph::~DataFlowGraph()
    {
        delete impl_;
    }
} // namespace dt::df
