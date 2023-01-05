#pragma once

#include <Callisto.h>

namespace MyApp
{
	class ExampleLayer2D : public Callisto::Layer
	{
	public:
		ExampleLayer2D();

		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Callisto::TimeStep timeStep) override;
		virtual void OnEvent(Callisto::Event& e) override;

		virtual void OnAttach() override;
	private:

		Callisto::Ref<Callisto::Texture2D> m_CheckerTexture;
		Callisto::OrthographicCameraController m_CameraController;
		float cubeRot = 0.0f;
	};
}

