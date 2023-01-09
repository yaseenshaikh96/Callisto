#include "ExampleLayerImGuiDocking.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace MyApp
{
	ExampleLayerImGuiDocking::ExampleLayerImGuiDocking()
		:
		Layer("Docking"),
		m_CameraController(1280.0f / 720.0f, true)
	{
		

	}
	void ExampleLayerImGuiDocking::OnAttach()
	{
		m_CheckerTexture = Callisto::Texture2D::Create("./Assets/Checkerboard.png");

		Callisto::FrameBufferSpecification specs;
		specs.Width = 1280.0f;
		specs.Height = 720.0f;
		m_FrameBuffer = Callisto::FrameBuffer::Create(specs);		
	}
	void ExampleLayerImGuiDocking::OnDetach()
	{

	}
	void ExampleLayerImGuiDocking::OnImGuiRender()
	{
		CALLISTO_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Callisto::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Callisto::Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndicesCount());

		ImGui::ColorEdit4("My Color", glm::value_ptr(m_Color));

		uint32_t frameBuffer = m_FrameBuffer->GetColorAttachmentID();
		float width = (float)m_FrameBuffer->GetSpecifications().Width;
		float height = (float)m_FrameBuffer->GetSpecifications().Height;

		//#pragma warning(suppress : 4312)
		ImGui::Image((void*)frameBuffer, ImVec2(320.0f, 280.0f));
		ImGui::End();

		ImGui::End();
	}

	void ExampleLayerImGuiDocking::OnUpdate(Callisto::TimeStep timeStep)
	{
		CALLISTO_PROFILE_FUNCTION();

		m_FrameBuffer->Bind();

		Callisto::RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
		Callisto::RenderCommand::Clear();

		m_CameraController.OnUpdate(timeStep);
		

		Callisto::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f), glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
		//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.0f), m_CheckerTexture, glm::vec2(10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		Callisto::Renderer2D::EndScene();
		m_FrameBuffer->UnBind();

	}

	void ExampleLayerImGuiDocking::OnEvent(Callisto::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}