#pragma once
#include <boost/graph/adjacency_list.hpp>
namespace gui::df
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
        VertexType type;
        std::string title;
    };
    using EdgeProperty = boost::property<boost::edge_index_t, int>;
    using Graph = boost::adjacency_list<boost::vecS,
                                        boost::vecS,
                                        boost::directedS,
                                        VertexInfo,
                                        EdgeProperty>;

    using VertexDesc = Graph::vertex_descriptor;
    using EdgeDesc = Graph::edge_descriptor;
} // namespace gui::df
