#pragma once
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

    struct EdgeInfo
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
} // namespace dt::df
