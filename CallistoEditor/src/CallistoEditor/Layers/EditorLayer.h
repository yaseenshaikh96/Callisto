#pragma once

#include <Callisto.h>

#include <entt.hpp>

#include "CallistoEditor/Panels/SceneHierarchyPanel.h"

namespace Callisto
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(TimeStep timeStep) override;
		virtual void OnEvent(Event& e) override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

	private:
		bool m_ViewPortFocused = false;
		float m_CubeRotation = 0.0f;
		Ref<Texture2D> m_CheckerTexture;
		glm::vec4 m_Color{0};
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewPortSize{0.0f, 0.0f};
		OrthographicCameraController m_CameraController;

		Entity m_CameraEntity;
		Entity m_SecondaryCameraEntity;

		bool m_PrimaryCamera = true;

		Ref<Scene> m_Scene;
		Entity m_SquareEntity;


		/* Panels */
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}