#include "CallistoPCH.h"
#include "ImguiLayer.h"

#include <imgui.h>
#include <Callisto/Platforms/OpenGL/ImguiOpenGLRenderer.h>

#include <Callisto/Application.h>

#include <GLFW/glfw3.h>
#include <Glad/glad.h>

namespace Callisto
{

	ImguiLayer::ImguiLayer()
		: Layer("DebugLayer")
	{}
	ImguiLayer::~ImguiLayer()
	{}

	void ImguiLayer::OnUpdate()
	{


		ImGuiIO& io{ ImGui::GetIO() };
		Application& app{ Application::Get() };
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());


		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : ( 1.0f / 60.0f );
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImguiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO io{ ImGui::GetIO() };
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Hazel key codes
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImguiLayer::OnDetach()
	{

	}

	void ImguiLayer::OnEvent(Event& e)
	{
		EventDispacther dispatcher{ e };
		dispatcher.Dispatch<MouseButtonPressedEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseScrollEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnMouseScrollEvent));
		dispatcher.Dispatch<MouseMovedEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnMouseMovedEvent));

		dispatcher.Dispatch<KeyPressedEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnKeyTypedEvent));
		
		dispatcher.Dispatch<WindowResizeEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnWindowResizeEvent));
		dispatcher.Dispatch<WindowCloseEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowMovedEvent>(CALLISTO_BIND_EVENT_FN(ImguiLayer::OnWindowMovedEvent));
	}

	bool ImguiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		io.MouseDown[e.GetButton()] = true;
		return false;
	}
	bool ImguiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		io.MouseDown[e.GetButton()] = false;
		return false;
	}
	bool ImguiLayer::OnMouseScrollEvent(MouseScrollEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		io.MouseWheel += e.GetOffsetY();
		io.MouseWheelH += e.GetOffsetX();
		return false;
	}
	bool ImguiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;

	}
	bool ImguiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		
		return false;
	}
	bool ImguiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		io.KeysDown[e.GetKeyCode()] = false;
		return false;
	}
	bool ImguiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		int keycode{ e.GetKeyCode() };
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);
		return false;
	}
	bool ImguiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		io.DisplaySize = ImVec2{(float)e.GetWidth(), (float)e.GetHeight()};
		io.DisplayFramebufferScale = ImVec2(1.0, 1.0);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}
	bool ImguiLayer::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		return false;
	}
	bool ImguiLayer::OnWindowMovedEvent(WindowMovedEvent& e)
	{
		ImGui::SetNextWindowPos(ImVec2(e.GetOffsetX(), e.GetOffsetY()));
		return false;
	}
}