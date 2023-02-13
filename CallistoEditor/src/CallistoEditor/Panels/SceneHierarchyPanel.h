#pragma once

#include <Callisto.h>
#include <Callisto/Scene/Scene.h>
#include <Callisto/Scene/Entity.h>

namespace Callisto
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
	
		void SetContext(const Ref<Scene>& context);

		void SetSelectionContext(Entity selectionContext) { m_SelectionContext = selectionContext; }
		Entity GetSelectionContext() const { return m_SelectionContext; }

		void OnImGuiRender();

	private:

		void DrawEntityNode(Entity entity);
		void DrawEntityProperties(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}