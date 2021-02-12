#include "data-flow/data_flow_editor.hpp"
#include <imgui.h>
#include <imnodes.h>

namespace dt::df
{
    class DataFlowEditor::Impl final
    {
    public:
        DataFlowGraph df_graph_;
    };

    DataFlowEditor::DataFlowEditor()
        : impl_{new Impl{}}
    {
    }

    void DataFlowEditor::render()
    {
        imnodes::BeginNodeEditor();

        impl_->df_graph_.render();

        imnodes::EndNodeEditor();
        { // add pending connections
            int started_at_attribute_id;
            int ended_at_attribute_id;
            if (imnodes::IsLinkCreated(&started_at_attribute_id, &ended_at_attribute_id))
            {
                impl_->df_graph_.addEdge(started_at_attribute_id, ended_at_attribute_id);
            }
        }
        { // delete connection
            int link_id;
            if (imnodes::IsLinkDestroyed(&link_id))
            {
                impl_->df_graph_.removeEdge(link_id);
            }
        }

        {
            const int num_selected = imnodes::NumSelectedNodes();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_Delete))
            {
                static std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<size_t>(num_selected));
                imnodes::GetSelectedNodes(selected_nodes.data());
                for (const int node_id : selected_nodes)
                {
                    impl_->df_graph_.removeNode(node_id);
                }
            }
        }
    }

    DataFlowGraph &DataFlowEditor::graph()
    {
        return impl_->df_graph_;
    }
    const DataFlowGraph &DataFlowEditor::graph() const
    {
        return impl_->df_graph_;
    }

    DataFlowEditor::~DataFlowEditor()
    {
        delete impl_;
    }
} // namespace dt::df
