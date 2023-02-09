#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace Callisto
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto enttEntity)
		{

			Entity e(enttEntity, m_Context.get());
			DrawEntityNode(e);
		});

		ImGui::End();
		ImGui::ShowDemoWindow();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if(m_SelectionContext == entity)
			flags = flags | ImGuiTreeNodeFlags_Selected;

		bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (open)
		{
			ImGui::TreePop();
		}
	}
}