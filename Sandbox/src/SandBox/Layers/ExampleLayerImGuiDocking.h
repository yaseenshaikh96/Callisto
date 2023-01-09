#pragma once

#include <Callisto.h>

namespace MyApp
{
	class ExampleLayerImGuiDocking : public Callisto::Layer
	{
	public:
		ExampleLayerImGuiDocking();
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Callisto::TimeStep timeStep) override;
		virtual void OnEvent(Callisto::Event& e) override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

	private:
		Callisto::Ref<Callisto::Texture2D> m_CheckerTexture;
		glm::vec4 m_Color;
		Callisto::Ref<Callisto::FrameBuffer> m_FrameBuffer;

		Callisto::OrthographicCameraController m_CameraController;

	};
}