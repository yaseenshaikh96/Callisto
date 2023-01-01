#include "CallistoPCH.h"

#include "Application.h"
#include <Callisto/Log.h>

#include <glad/glad.h>

#include <Callisto/Input.h>

//#include <Callisto/Renderer/Shader.h>

namespace Callisto
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;
	 


	Application::Application()
	{
		CALLISTO_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImguiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[]
		{
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer1{ VertexBuffer::Create(vertices, sizeof(vertices)) };
		BufferLayout layout
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer1->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer1);
		unsigned int indices[] { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer1{ IndexBuffer::Create(indices, 3) };
		m_VertexArray->SetIndexBuffer(indexBuffer1);


		m_SquareVA.reset(VertexArray::Create());
		float vertices2[]
		{
			-0.75f, -0.75f,  0.0f,
			 0.75f, -0.75f,  0.0f,
			 0.75f,  0.75f,  0.0f,
			-0.75f,  0.75f,  0.0f
		};
		std::shared_ptr<VertexBuffer> SquareVB{ VertexBuffer::Create(vertices2, 12 * sizeof(float) ) };
		BufferLayout layout2
		{
			{ShaderDataType::Float3, "a_Position"}
		};
		SquareVB->SetLayout(layout2);
		m_SquareVA->AddVertexBuffer(SquareVB);
		unsigned int indices2[6]{ 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> SquareVI{ IndexBuffer::Create(indices2, 6) };
		m_SquareVA->SetIndexBuffer(SquareVI);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
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
		m_ShaderSquare.reset(new Shader(vertexSrc2, fragmentSrc2));
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispacther dispatcher{ e };
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	
		//CALLISTO_CORE_TRACE("e: {0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*(--it))->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.4f, 0.01f, 0.5f, 1.0f); // GOOD PURPLE
			glClear(GL_COLOR_BUFFER_BIT);
			
			m_ShaderSquare->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}


}