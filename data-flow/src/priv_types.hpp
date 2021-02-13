#pragma once
#include <shared_mutex>
#include <atomic>
#include <memory>
#include "data-flow/types.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/signals2.hpp>

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

    struct RefCon
    {
        boost::signals2::connection connection;
        ~RefCon();
    };
    struct EdgeInfo
    {
        EdgeId id;
        std::shared_ptr<RefCon> connection;
    };
    struct EdgeInfo_t
    {
        typedef boost::edge_property_tag kind;
    };

    using EdgeProperty = boost::property<EdgeInfo_t, EdgeInfo>;
    using Graph = boost::adjacency_list<boost::vecS,
                                        boost::vecS,
                                        boost::bidirectionalS,
                                        VertexInfo,
                                        EdgeProperty>;

    using VertexDesc = Graph::vertex_descriptor;
    using EdgeDesc = Graph::edge_descriptor;
} // namespace dt::df
