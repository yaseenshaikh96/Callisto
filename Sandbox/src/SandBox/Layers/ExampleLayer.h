#pragma once

#include <Callisto.h>

namespace MyApp
{
	class ExampleLayer : public Callisto::Layer
	{
	public:
		ExampleLayer();
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Callisto::TimeStep timeStep) override;
		virtual void OnEvent(Callisto::Event& e) override;

		virtual void OnAttach() override;
		//virtual void OnDettach() override;

	private:

		Callisto::ShaderLibrary m_ShaderLibrary;
		Callisto::Ref<Callisto::VertexArray> m_SquareVA;
		Callisto::Ref<Callisto::Texture2D> m_TextureCheckerBoard;
		Callisto::Ref<Callisto::Texture2D> m_TextureChernoLogo;

		Callisto::OrthographicCameraController m_CameraController;

		glm::vec4 m_SquareColor;
	};
}