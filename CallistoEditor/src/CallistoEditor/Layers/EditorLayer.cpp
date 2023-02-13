#include "EditorLayer.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include "ImGuizmo.h"

#include "Callisto/Math/Math.h"
#include "Callisto/Scene/SceneSerializer.h"
#include "Callisto/Utils/PlatformUtils.h"

namespace Callisto
{
	EditorLayer::EditorLayer()
		:
		Layer("EditorLayer"),
		//m_CameraController(1.77f, true)
		m_EditorCamera()
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

		m_EditorCamera = EditorCamera(30, (float)specs.Width / specs.Height, 0.01f, 1000.0f);

		m_SceneHierarchyPanel.SetContext(m_Scene);
		
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
			
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				NewScene();
			}
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				OpenScene();
			}
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				SaveSceneAs();
			}

			if (ImGui::MenuItem("Exit")) 
			{
				Application::Get().Close();
			}

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
		m_ViewPortHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockImGuiEvent(!m_ViewPortFocused && !m_ViewPortHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t frameBuffer = m_FrameBuffer->GetColorAttachmentID();
		#pragma warning(push)
		#pragma warning(disable : 4312)
		ImGui::Image((void*)frameBuffer, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		#pragma warning(pop)
		
		//Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectionContext();
		if (selectedEntity && m_ImGuizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);


			bool snapping = Input::IsKeyPressed((int)Key::LeftControl);
			float snapValue = 0.5f;
			if (m_ImGuizmoType == (int)ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}
			float snapValues[3] = { snapValue, snapValue, snapValue };

			//Runtime Camera
			//Entity cameraEntity = m_Scene->GetPrimaryCameraEntity();
			//SceneCamera& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//TransformComponent& tc = cameraEntity.GetComponent<TransformComponent>();
			//glm::mat4 cameraView = glm::inverse(tc.GetTransform());
			//const glm::mat4& cameraProjection = camera.GetProjection();
			
			//Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();

			//entity
			TransformComponent& entityTc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 entityTransform = entityTc.GetTransform();

			ImGuizmo::Manipulate(
				glm::value_ptr(cameraView), 
				glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_ImGuizmoType,
				ImGuizmo::LOCAL,
				glm::value_ptr(entityTransform),
				nullptr,
				snapping ? snapValues : nullptr);
			
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(
					entityTransform, 
					position,
					rotation,
					scale);
				
				entityTc.Position = position;
				
				glm::vec3 deltaRotation = rotation - entityTc.Rotation;
				entityTc.Rotation += deltaRotation;

				entityTc.Scale = scale;
			
			}
			

		}
		
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
			//m_CameraController.OnResize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_EditorCamera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_Scene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}

		if(m_ViewPortFocused)
		{
			m_EditorCamera.OnUpdate(timeStep);
			//m_CameraController.OnUpdate(timeStep);
		}
		{
			CALLISTO_PROFILE_SCOPE("Render");
			Renderer2D::ResetStatistics();
			
			m_FrameBuffer->Bind();
			RenderCommand::SetClearColor({ 0.2f, 0.1f, 0.2f, 1.0f });
			RenderCommand::Clear();

			m_Scene->OnUpdateEditor(timeStep, m_EditorCamera);
			//m_Scene->OnUpdateRuntime(timeStep);
			m_FrameBuffer->UnBind();
		}

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
		
		EventDispacther dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(CALLISTO_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
		{
			return false;
		}

		bool ctrlPressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shiftPressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		

		switch ((Key)e.GetKeyCode())
		{
		case Key::N:
		{
			if (ctrlPressed)
			{
				NewScene();
			}
			break;
		}
		case Key::O:
		{
			if (ctrlPressed)
			{
				OpenScene();
			}
			break;
		}
		case Key::S:
		{
			if (ctrlPressed && shiftPressed)
			{
				SaveSceneAs();
			}
			break;
		}

		//ImGuizmo
		case Key::Q:
		{
			m_ImGuizmoType = -1;
			break;
		}
		case Key::W:
		{
			m_ImGuizmoType = (int)ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			m_ImGuizmoType = (int)ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			m_ImGuizmoType = (int)ImGuizmo::OPERATION::SCALE;
			break;
		}

		default:
			break;
		}
		return false;
	}

	/*****************************************************************************************************************************************/
	/* Save Scene Commands */
	/*****************************************************************************************************************************************/
	void EditorLayer::NewScene()
	{
		m_Scene = CreateRef<Scene>();
		m_Scene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_SceneHierarchyPanel.SetContext(m_Scene);
	}
	void EditorLayer::OpenScene()
	{
		std::string filePath = FileDialogs::OpenFile("Callisto Scene (*.callisto)\0*.callisto\0");
		if (!filePath.empty())
		{
			m_Scene = CreateRef<Scene>();
			m_Scene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_SceneHierarchyPanel.SetContext(m_Scene);

			SceneSerializer sceneSerializer(m_Scene);
			sceneSerializer.DeSerialize(filePath);
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filePath = FileDialogs::OpenFile("Callisto Scene (*.callisto)\0*.callisto\0");
		if (!filePath.empty())
		{
			SceneSerializer sceneSerializer(m_Scene);
			sceneSerializer.Serialize(filePath);
		}
	}
}