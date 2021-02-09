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
        VertexType type;
    };
    using EdgeProperty = boost::property<boost::edge_index_t, int>;
    using Graph = boost::adjacency_list<boost::vecS,
                                        boost::vecS,
                                        boost::directedS,
                                        VertexInfo,
                                        EdgeProperty>;
} // namespace gui::df
