#pragma once

#include <Callisto.h>

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
		Ref<Texture2D> m_CheckerTexture;
		glm::vec4 m_Color;
		Ref<FrameBuffer> m_FrameBuffer;

		OrthographicCameraController m_CameraController;

	};
}