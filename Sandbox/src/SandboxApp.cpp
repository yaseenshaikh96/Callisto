#include <Callisto.h>

class ExampleLayer : public Callisto::Layer
{
public:
	ExampleLayer()
		:
		Layer("Example"),
		m_Camera( - 1.6f, 1.6f, -0.9f, 0.9f )
	{

		m_VertexArray.reset(Callisto::VertexArray::Create());

		float vertices[] =
		{
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		std::shared_ptr<Callisto::VertexBuffer> vertexBuffer1 = std::shared_ptr<Callisto::VertexBuffer>(Callisto::VertexBuffer::Create(vertices, sizeof(vertices)));
		Callisto::BufferLayout layout =
		{
			{Callisto::ShaderDataType::Float3, "a_Position"},
			{Callisto::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer1->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer1);
		unsigned int indices[] = { 0, 1, 2 };
		std::shared_ptr<Callisto::IndexBuffer> indexBuffer1 = std::shared_ptr<Callisto::IndexBuffer>(Callisto::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer1);


		m_SquareVA.reset(Callisto::VertexArray::Create());
		float vertices2[] =
		{
			-0.75f, -0.75f,  0.0f,
			 0.75f, -0.75f,  0.0f,
			 0.75f,  0.75f,  0.0f,
			-0.75f,  0.75f,  0.0f
		};
		std::shared_ptr<Callisto::VertexBuffer> SquareVB = std::shared_ptr<Callisto::VertexBuffer>(Callisto::VertexBuffer::Create(vertices2, 12 * sizeof(float)));
		Callisto::BufferLayout layout2 =
		{
			{Callisto::ShaderDataType::Float3, "a_Position"}
		};
		SquareVB->SetLayout(layout2);
		m_SquareVA->AddVertexBuffer(SquareVB);
		unsigned int indices2[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Callisto::IndexBuffer> SquareVI = std::shared_ptr<Callisto::IndexBuffer>(Callisto::IndexBuffer::Create(indices2, 6));
		m_SquareVA->SetIndexBuffer(SquareVI);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
		m_Shader.reset(new Callisto::Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}	
		)";
		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(1.0, 1.0, 0.0, 1.0);
			}	
		)";
		m_ShaderSquare.reset(new Callisto::Shader(vertexSrc2, fragmentSrc2));

	}

	void OnImGuiRender() override
	{
	}

	void OnUpdate() override
	{
		Callisto::RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
		Callisto::RenderCommand::Clear();

		{
			float inc{ 0.02f };
			if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_W))
				camPos.y += inc;
			else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_S))
				camPos.y -= inc;
			if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_A))
				camPos.x -= inc;
			else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_D))
				camPos.x += inc;

			if (Callisto::Input::IsMouseButtonPressed(0))
				camRot += 1;
			else if (Callisto::Input::IsMouseButtonPressed(1))
				camRot -= 1;
		}

		m_Camera.SetPosition(camPos);
		m_Camera.SetRotation(camRot);

		Callisto::Renderer::BeginScene(m_Camera);

		Callisto::Renderer::Submit(m_ShaderSquare, m_SquareVA);
		Callisto::Renderer::Submit(m_Shader, m_VertexArray);

		Callisto::Renderer::EndScene();
	}
	void OnEvent(Callisto::Event& e) override
	{
	}

private:
	glm::vec3 camPos{};
	float camRot = 0;

	std::shared_ptr<Callisto::Shader> m_Shader;
	std::shared_ptr<Callisto::Shader> m_ShaderSquare;
	std::shared_ptr<Callisto::VertexArray> m_VertexArray;
	std::shared_ptr<Callisto::VertexArray> m_SquareVA;
	Callisto::OrthographicCamera m_Camera;
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