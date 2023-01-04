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
		Callisto::ShaderLibrary m_ShaderLibrary;
		Callisto::Ref<Callisto::VertexArray> m_SquareVA;
		glm::vec4 m_SquareColor;

		Callisto::OrthographicCameraController m_CameraController;

	};
}

