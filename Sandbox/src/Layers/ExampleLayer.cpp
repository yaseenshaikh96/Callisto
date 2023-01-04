#include "ExampleLayer.h"

#include <Callisto/Platforms/OpenGL/OpenGLShader.h>
#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace MyApp
{
	ExampleLayer::ExampleLayer()
		:
		Layer("Example"),
		m_CameraController(1280.0f / 720.0f, true),
		m_SquareColor(0.8f, 0.2f, 0.2f, 1.0f)
	{
	}
	void ExampleLayer::OnAttach()
	{
		m_VertexArray = Callisto::VertexArray::Create();

		float vertices[] =
		{
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		Callisto::Ref<Callisto::VertexBuffer> vertexBuffer1 = Callisto::VertexBuffer::Create(vertices, sizeof(vertices));
		Callisto::BufferLayout layout =
		{
			{Callisto::ShaderDataType::Float3, "a_Position"},
			{Callisto::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer1->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer1);
		unsigned int indices[] = { 0, 1, 2 };
		Callisto::Ref<Callisto::IndexBuffer> indexBuffer1 = Callisto::IndexBuffer::Create(indices, 3);
		m_VertexArray->SetIndexBuffer(indexBuffer1);


		m_SquareVA = Callisto::VertexArray::Create(); 
		float verticesSquare[] =
		{
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 1.0f
		};
		Callisto::Ref<Callisto::VertexBuffer> SquareVB = Callisto::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
		Callisto::BufferLayout layout2 =
		{
			{Callisto::ShaderDataType::Float3, "a_Position"},
			{Callisto::ShaderDataType::Float2, "a_TexCoord"}
		};
		SquareVB->SetLayout(layout2);
		m_SquareVA->AddVertexBuffer(SquareVB);
		unsigned int indices2[6] = { 0, 1, 2, 2, 3, 0 };
		Callisto::Ref<Callisto::IndexBuffer> SquareVI = Callisto::IndexBuffer::Create(indices2, 6);
		m_SquareVA->SetIndexBuffer(SquareVI);

		std::string triangleVertexSrc = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				}	
			)";

		std::string triangleFragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = v_Color;
				}	
			)";
		m_TriangleShader = Callisto::Shader::Create("Triangle", triangleVertexSrc, triangleFragmentSrc);

		auto flatColorShader = m_ShaderLibrary.Load("./Assets/Shaders/FlatColor.glsl");
		auto textureShader = m_ShaderLibrary.Load("./Assets/Shaders/Texture.glsl");

		m_TextureCheckerBoard = Callisto::Texture2D::Create("./Assets/Checkerboard.png");
		m_TextureChernoLogo = Callisto::Texture2D::Create("./Assets/ChernoLogo.png");

		textureShader->Bind();
		dynamic_cast<Callisto::OpenGLShader*>(textureShader.get())->UploadUniformInt("u_Texture", 0);
		//dynamic_cast<Callisto::OpenGLShader*>(m_ShaderTexture.get())->UploadUniformInt("u_Texture", 0);

	}

	void ExampleLayer::OnImGuiRender()
	{
		ImGui::Begin("Colors");
		ImGui::ColorEdit4("square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void ExampleLayer::OnUpdate(Callisto::TimeStep timeStep)
	{

		CALLISTO_INFO("Time: {0}ms", timeStep.GetMilliseconds());

		Callisto::RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
		Callisto::RenderCommand::Clear();

		m_CameraController.OnUpdate(timeStep);
		Callisto::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		Callisto::Ref<Callisto::Shader> flatColorShader = m_ShaderLibrary.Get("FlatColor");
		flatColorShader->Bind();
		dynamic_cast<Callisto::OpenGLShader*>(flatColorShader.get())->UploadUniformFloat4("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), pos) * scale;
				Callisto::Renderer::Submit(flatColorShader, m_SquareVA, transform2);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_TextureCheckerBoard->Bind();
		Callisto::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//Callisto::Renderer::Submit(m_ShaderTexture, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_TextureChernoLogo->Bind();
		Callisto::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Callisto::Renderer::Submit(m_Shader, m_VertexArray);

		Callisto::Renderer::EndScene();
	}

	void ExampleLayer::OnEvent(Callisto::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}