#include "ExampleLayer2D.h"

#include <Callisto/Platforms/OpenGL/OpenGLShader.h>
#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MyApp
{
	ExampleLayer2D::ExampleLayer2D()
		:
		Layer("Example2D"),
		m_CameraController(1280.0f / 720.0f, true),
		m_SquareColor(0.8f, 0.2f, 0.2f, 1.0f)
	{
	}

	void ExampleLayer2D::OnAttach()
	{
		float verticesSquare[] =
		{
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f
		};
		unsigned int indices2[6] = { 0, 1, 2, 2, 3, 0 };

		m_SquareVA = Callisto::VertexArray::Create();
		Callisto::Ref<Callisto::VertexBuffer> SquareVB = Callisto::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
		Callisto::BufferLayout layout2 =
		{
			{Callisto::ShaderDataType::Float3, "a_Position"},
			{Callisto::ShaderDataType::Float2, "a_TexCoord"}
		};
		SquareVB->SetLayout(layout2);
		m_SquareVA->AddVertexBuffer(SquareVB);

		Callisto::Ref<Callisto::IndexBuffer> SquareVI = Callisto::IndexBuffer::Create(indices2, 6);
		m_SquareVA->SetIndexBuffer(SquareVI);

		auto flatColorShader = m_ShaderLibrary.Load("./Assets/Shaders/FlatColor.glsl");
	}

	void ExampleLayer2D::OnImGuiRender()
	{
		ImGui::Begin("Colors");
		ImGui::ColorEdit4("square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void ExampleLayer2D::OnUpdate(Callisto::TimeStep timeStep)
	{
		Callisto::RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
		Callisto::RenderCommand::Clear();

		m_CameraController.OnUpdate(timeStep);

		Callisto::Renderer::BeginScene(m_CameraController.GetCamera());


		Callisto::Ref<Callisto::Shader> flatColorShader = m_ShaderLibrary.Get("FlatColor");
		flatColorShader->Bind();
		dynamic_cast<Callisto::OpenGLShader*>(flatColorShader.get())->UploadUniformFloat4("u_Color", m_SquareColor);

		Callisto::Renderer::Submit(flatColorShader, m_SquareVA);

		/*
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), pos) * scale;
				Callisto::Renderer::Submit(flatColorShader, m_SquareVA, transform2);
			}
		}
		*/

		Callisto::Renderer::EndScene();
	}

	void ExampleLayer2D::OnEvent(Callisto::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}