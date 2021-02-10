#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <boost/graph/adjacency_list.hpp>
#include <boost/signals2.hpp>
#include "nodes/base_node.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    enum class VertexType
    {
        node,
        input,
        output
    };
    struct VertexInfo
    {
        int id;
        NodeId parent_id; //! -1 when type == VertexType::node
        VertexType type;
    };

    using EdgeId = int;
    struct DATAFLOW_EXPORT EdgeInfo
    {
        EdgeId id;
        boost::signals2::connection connection;
        //! \todo we need to make sure if the destructor is really only called when the edge gets destroyed. if not look at scoped_connection
        ~EdgeInfo();
    };
    struct EdgeInfo_t
    {
        typedef boost::edge_property_tag kind;
    };

    using EdgeProperty = boost::property<EdgeInfo_t, EdgeInfo>;
    using Graph = boost::adjacency_list<boost::vecS,
                                        boost::vecS,
                                        boost::directedS,
                                        VertexInfo,
                                        EdgeProperty>;

    using VertexDesc = Graph::vertex_descriptor;
    using EdgeDesc = Graph::edge_descriptor;

    using NodePtr = std::shared_ptr<BaseNode>;
    using NodeIdGenerator = std::atomic_int;
    using SlotIdGenerator = std::atomic_int;
    using NodeFactory = std::function<NodePtr(NodeIdGenerator &, SlotIdGenerator &)>;
} // namespace dt::df
