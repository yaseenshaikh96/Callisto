#pragma once

#include <Callisto.h>

#include <entt.hpp>

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
		glm::vec4 m_Color;
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewPortSize{0.0f, 0.0f};
		OrthographicCameraController m_CameraController;

		Ref<Scene> m_Scene;
		entt::entity m_SquareEntity;
	};
}