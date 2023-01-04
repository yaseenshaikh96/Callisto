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
		glm::vec3 camPos{};
		float camRot = 0;

		Callisto::ShaderLibrary m_ShaderLibrary;

		Callisto::Ref<Callisto::Shader> m_TriangleShader;
		Callisto::Ref<Callisto::VertexArray> m_VertexArray;
		Callisto::Ref<Callisto::VertexArray> m_SquareVA;
		Callisto::OrthographicCameraController m_CameraController;

		Callisto::Ref<Callisto::Texture2D> m_TextureCheckerBoard;
		Callisto::Ref<Callisto::Texture2D> m_TextureChernoLogo;

		glm::vec4 m_SquareColor;
	};
}