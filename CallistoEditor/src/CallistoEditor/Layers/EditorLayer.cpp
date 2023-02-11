#include "EditorLayer.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include "Callisto/Scene/SceneSerializer.h"

namespace Callisto
{
	EditorLayer::EditorLayer()
		:
		Layer("EditorLayer"),
		m_CameraController(1280.0f / 720.0f, true)
	{
		
	}

	void EditorLayer::OnAttach()
	{
		m_CheckerTexture = Texture2D::Create("./Assets/Checkerboard.png");

		FrameBufferSpecification specs;
		specs.Width = 1600;
		specs.Height = 900;
		m_FrameBuffer = FrameBuffer::Create(specs);		

		m_Scene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_Scene);
		
		/*
		m_SquareEntity = m_Scene->CreateEntity("Square");
		m_SquareEntity.GetComponent<TransformComponent>().Position = glm::vec3(-1.0f, 0.0f, 0.0f);
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

		Entity SquareEntity2 = m_Scene->CreateEntity("Square2");
		SquareEntity2.GetComponent<TransformComponent>().Position = glm::vec3(1.0f, 0.0f, 0.0f);
		SquareEntity2.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));

		m_CameraEntity = m_Scene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondaryCameraEntity = m_Scene->CreateEntity("Secondary Camera Entity");
		m_SecondaryCameraEntity.AddComponent<CameraComponent>().Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
			}
			virtual void OnUpdate(TimeStep timeStep) override
			{
				auto& position = GetComponent<TransformComponent>().Position;
				float speed = 2;
				if (Input::IsKeyPressed(CALLISTO_KEY_A))
					position.x -= speed * timeStep;
				if (Input::IsKeyPressed(CALLISTO_KEY_D))
					position.x += speed * timeStep;
				if (Input::IsKeyPressed(CALLISTO_KEY_W))
					position.y += speed * timeStep;
				if (Input::IsKeyPressed(CALLISTO_KEY_S))
					position.y -= speed * timeStep;

			}
			virtual void OnDestroy() override
			{}
		};
		m_SecondaryCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		*/
	}

	void EditorLayer::OnDetach()
	{

	}
	
	void EditorLayer::OnImGuiRender()
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
		ImGuiStyle& style = ImGui::GetStyle();
		float previousWindowMinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 270.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = previousWindowMinSizeX;

		ImGui::BeginMenuBar();
		if(ImGui::BeginMenu("File"))
		{
			
			
			if (ImGui::MenuItem("Serialize"))
			{
				SceneSerializer sceneSerializer(m_Scene);
				sceneSerializer.Serialize("Assets/Scenes/Example.callisto");
			}
			if (ImGui::MenuItem("Deserialize"))
			{
				SceneSerializer sceneSerializer(m_Scene);
				sceneSerializer.DeSerialize("Assets/Scenes/Example.callisto");
			}

			if (ImGui::MenuItem("Exit")) Application::Get().Close();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Renderer2D Stats");
		auto stats = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndicesCount());
		ImGui::End(); // Renderer2D Stats


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort");
		m_ViewPortFocused = ImGui::IsWindowFocused();
		Application::Get().GetImGuiLayer()->SetBlockImGuiEvent(!m_ViewPortFocused);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t frameBuffer = m_FrameBuffer->GetColorAttachmentID();
		#pragma warning(push)
		#pragma warning(disable : 4312)
		ImGui::Image((void*)frameBuffer, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		#pragma warning(pop)
		ImGui::End();
		ImGui::PopStyleVar(); // viewport

		ImGui::End(); // docking
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		CALLISTO_PROFILE_FUNCTION();

		// resize viewport
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecifications();
			m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_CameraController.OnResize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_Scene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}

		if(m_ViewPortFocused)
		{
			m_CameraController.OnUpdate(timeStep);
		}
		
		{
			CALLISTO_PROFILE_SCOPE("Render");
			Renderer2D::ResetStatistics();
			
			m_FrameBuffer->Bind();
			RenderCommand::SetClearColor({ 0.2f, 0.1f, 0.2f, 1.0f });
			RenderCommand::Clear();
			m_Scene->OnUpdate(timeStep);
			m_FrameBuffer->UnBind();
		}

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}