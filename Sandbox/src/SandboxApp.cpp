#include <Callisto.h>

#include <Callisto/Platforms/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Imgui/imgui.h>
#include <Callisto/Renderer/Texture.h>

class ExampleLayer : public Callisto::Layer
{
public:
	ExampleLayer()
		:
		Layer("Example"),
		m_Camera( - 1.6f, 1.6f, -0.9f, 0.9f ),
		m_Position2(0.0f),
		m_SquareColor(0.8f, 0.2f, 0.2f, 1.0f)
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

		std::string vertexSrc = R"(
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

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}	
		)";
		m_Shader.reset(Callisto::Shader::Create(vertexSrc, fragmentSrc));

		std::string vertexSrcSq = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			uniform vec4 u_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Color = u_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";
		std::string fragmentSrcSq = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec4 v_Color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(1.0, 1.0, 0.0, 1.0);
				color = v_Color;
			}	
		)";
		m_ShaderSquare.reset(Callisto::Shader::Create(vertexSrcSq, fragmentSrcSq));
		m_ShaderTexture.reset(Callisto::Shader::Create("./Assets/Shaders/Texture.glsl"));

		m_Texture = Callisto::Texture2D::Create("./Assets/Checkerboard.png");
		m_TextureTransparent = Callisto::Texture2D::Create("./Assets/ChernoLogo.png");
		
		
		//m_ShaderTexture->Bind();
		m_ShaderTexture->Bind();
		dynamic_cast<Callisto::OpenGLShader*>(m_ShaderTexture.get())->UploadUniformInt("u_Texture", 0);
		
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Colors");
		ImGui::ColorEdit4("square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnUpdate(Callisto::TimeStep timeStep) override
	{

		//CALLISTO_INFO("Time: {0}ms", timeStep.GetMilliseconds());

		Callisto::RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
		Callisto::RenderCommand::Clear();

		{
			float inc{ 3.0f };
			if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_W))
				camPos.y += inc * timeStep;
			else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_S))
				camPos.y -= inc * timeStep;
			if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_A))
				camPos.x -= inc * timeStep;
			else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_D))
				camPos.x += inc * timeStep;

			if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_I))
				m_Position2.y += inc * timeStep * 0.5f;
			else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_K))
				m_Position2.y -= inc * timeStep * 0.5f;
			if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_J))
				m_Position2.x -= inc * timeStep * 0.5f;
			else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_L))
				m_Position2.x += inc * timeStep * 0.5f;

			if (Callisto::Input::IsMouseButtonPressed(0))
				camRot += 90 * timeStep;
			else if (Callisto::Input::IsMouseButtonPressed(1))
				camRot -= 90 * timeStep;
		}

		m_Camera.SetPosition(camPos);
		m_Camera.SetRotation(camRot);


		Callisto::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_ShaderSquare->Bind();
		dynamic_cast<Callisto::OpenGLShader*>(m_ShaderSquare.get())->UploadUniformFloat4("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), pos) * scale;
				Callisto::Renderer::Submit(m_ShaderSquare, m_SquareVA, transform2);
			}
		}

		m_Texture->Bind();
		Callisto::Renderer::Submit(m_ShaderTexture, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_TextureTransparent->Bind();
		Callisto::Renderer::Submit(m_ShaderTexture, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		// Triangle
		//Callisto::Renderer::Submit(m_Shader, m_VertexArray);

		Callisto::Renderer::EndScene();
	}
	void OnEvent(Callisto::Event& e) override
	{
	}

private:
	glm::vec3 camPos{};
	float camRot = 0;

	Callisto::Ref<Callisto::Shader> m_Shader;
	Callisto::Ref<Callisto::Shader> m_ShaderSquare;
	Callisto::Ref<Callisto::Shader> m_ShaderTexture;
	Callisto::Ref<Callisto::VertexArray> m_VertexArray;
	Callisto::Ref<Callisto::VertexArray> m_SquareVA;
	Callisto::OrthographicCamera m_Camera;

	Callisto::Ref<Callisto::Texture2D> m_Texture;
	Callisto::Ref<Callisto::Texture2D> m_TextureTransparent;

	glm::vec4 m_SquareColor;

	glm::vec3 m_Position2;
};

class Sandbox : public Callisto::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};


Callisto::Application* Callisto::CreateApplication()
{
	return new Sandbox();
}
