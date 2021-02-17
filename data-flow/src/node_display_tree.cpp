#include "node_display_tree.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <iostream>
namespace dt::df
{
    struct my_visitor : boost::default_dfs_visitor
    {
        const NodeDisplayDrawFnc &draw_fnc_;
        mutable int curr_level = 0;
        explicit my_visitor(const NodeDisplayDrawFnc &draw_fnc)
            : draw_fnc_{draw_fnc}
        {
        }
        void discover_vertex(const NodeTree::vertex_descriptor &s, const NodeTree &g) const
        {
            const auto prev_lvl = curr_level++;
            draw_fnc_(prev_lvl, curr_level, boost::out_degree(s, g) == 0, g[s].node_key, g[s].display_name);
        }

        void finish_vertex(const NodeTree::vertex_descriptor &s, const NodeTree &g) const
        {
            draw_fnc_(curr_level, curr_level, boost::out_degree(s, g) == 0, g[s].node_key, g[s].display_name);
            curr_level--;
        }
    };

    NodeDisplayGraph::NodeDisplayGraph()
    {
        root_node_ = boost::add_vertex({"root", "Nodes"}, node_tree_);
    }

    void NodeDisplayGraph::addNode(const std::string &node_key, const std::string &node_name)
    {
        std::vector<std::string> groups;
        boost::split(groups, node_name, boost::is_any_of("/"));
        if (groups.size() == 0)
        {
            addNodeToGraph(node_key, node_name, root_node_);
            return;
        }

        Desc parent = root_node_;
        for (int i = 0; i < groups.size() - 1; i++)
        {
            parent = addNodeToGraph("", groups[i], parent);
        }
        addNodeToGraph(node_key, groups[groups.size() - 1], parent);
    }

    NodeDisplayGraph::Desc NodeDisplayGraph::addNodeToGraph(const std::string &node_key, const std::string &node_name, const Desc parent)
    {
        // look if we have already a node with the same name. If yes, return the already existing node
        for (const auto vd : boost::make_iterator_range(boost::adjacent_vertices(parent, node_tree_)))
        {
            auto &node = node_tree_[vd];
            if (node.display_name == node_name)
            {
                if (node.node_key != node_key)
                {
                    node.node_key = node_key;
                    //! \todo log key change!!
                }
                return vd;
            }
        }
        const auto desc = boost::add_vertex({node_key, node_name}, node_tree_);
        boost::add_edge(parent, desc, node_tree_);
        return desc;
    }

    void NodeDisplayGraph::drawTree(const NodeDisplayDrawFnc &draw_fnc) const
    {
        /* INDEX MAP CREATION*/
        std::map<Desc, size_t> i_map;
        for (auto v : boost::make_iterator_range(boost::vertices(node_tree_)))
        {
            i_map.emplace(v, i_map.size());
        }

        auto ipmap = boost::make_assoc_property_map(i_map);

        /* COLOR MAP CREATION */
        std::vector<boost::default_color_type> c_map(boost::num_vertices(node_tree_));
        auto cpmap = boost::make_iterator_property_map(c_map.begin(), ipmap);

        const my_visitor vis{draw_fnc};
        boost::depth_first_search(node_tree_, boost::visitor(vis)
                                                  .vertex_index_map(ipmap)
                                                  .color_map(cpmap));
    }

}
