#include "EditorLayer.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp>


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
		specs.Width = static_cast<uint32_t>(1280.0f);
		specs.Height = static_cast<uint32_t>(720.0f);
		m_FrameBuffer = FrameBuffer::Create(specs);		


		m_Scene = CreateRef<Scene>();

		m_SquareEntity = m_Scene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

		m_CameraEntity = m_Scene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondaryCameraEntity = m_Scene->CreateEntity("Secondary Camera Entity");
		m_SecondaryCameraEntity.AddComponent<CameraComponent>().Primary = false;

		//m_SquareEntity.HasComponent<TransformComponent>();


		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				CALLISTO_CORE_INFO("Created!");
			}
			virtual void OnUpdate(TimeStep timeStep) override
			{
				CALLISTO_CORE_INFO("TimeStep: {0}", timeStep);
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 2;
				if (Input::IsKeyPressed(CALLISTO_KEY_A))
					transform[3][0] -= speed * timeStep;
				if (Input::IsKeyPressed(CALLISTO_KEY_D))
					transform[3][0] += speed * timeStep;
				if (Input::IsKeyPressed(CALLISTO_KEY_W))
					transform[3][1] += speed * timeStep;
				if (Input::IsKeyPressed(CALLISTO_KEY_S))
					transform[3][1] -= speed * timeStep;

			}
			virtual void OnDestroy() override
			{}
		};
		m_SecondaryCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndicesCount());

		ImGui::Separator();
		ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());
		auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
		ImGui::ColorEdit4("My Color", glm::value_ptr(squareColor));


		ImGui::DragFloat3("Camera Transform: ", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
		
		if (ImGui::Checkbox("Use Primary", &m_PrimaryCamera))
		{
			m_SecondaryCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
		}
		auto& secondCamera = m_SecondaryCameraEntity.GetComponent<CameraComponent>().Camera;
		float cameraSize = secondCamera.GetOrthographicsSize();
		if(ImGui::DragFloat("Second Camera Size: ", &cameraSize))
		{
			secondCamera.SetOrthographicsSize(cameraSize);
		}

		ImGui::End(); // settings


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort");

		m_ViewPortFocused = ImGui::IsWindowFocused();
		Application::Get().GetImGuiLayer()->SetBlockImGuiEvent(!m_ViewPortFocused);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t frameBuffer = m_FrameBuffer->GetColorAttachmentID();
		ImGui::Image((void*)frameBuffer, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar(); // viewport

		ImGui::End(); // docking
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		CALLISTO_PROFILE_FUNCTION();


		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecifications();
			m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_CameraController.OnResize(m_ViewPortSize.x, m_ViewPortSize.y);

			m_Scene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}

		{
			CALLISTO_PROFILE_SCOPE("Renderer start");
			Renderer2D::ResetStatistics();
			m_FrameBuffer->Bind();
			RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
			RenderCommand::Clear();
		}

		if(m_ViewPortFocused)
		{
			m_CameraController.OnUpdate(timeStep);
		}
		
		//m_CubeRotation += 90 * timeStep;
		{
			CALLISTO_PROFILE_SCOPE("Render");
			
			m_Scene->OnUpdate(timeStep);
			
			/*
			Renderer2D::DrawRotatedQuadFilled(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec2(0.5f, 0.5f),
				m_CubeRotation,
				glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));

			Renderer2D::DrawRotatedQuadFilled(
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec2(0.5f, 0.5f),
				m_CubeRotation,
				glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
		
			Renderer2D::DrawRotatedQuadFilled(
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec2(0.5f, 0.5f),
				m_CubeRotation,
				glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
			*/
			m_FrameBuffer->UnBind();
		}

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}