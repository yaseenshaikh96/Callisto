#include <Callisto.h>
#include <Imgui/imgui.h>

class ExampleLayer : public Callisto::Layer
{
public:
	ExampleLayer()
		:
		Layer("Example")
	{
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test!");
		ImGui::Text("Hello ImGui!!");
		ImGui::End();
	}

	void OnUpdate() override
	{
		if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_E))
			CALLISTO_CORE_TRACE("E is Pressed");

		//CALLISTO_INFO("ExampleLayer::OnUpdate");
	}
	void OnEvent(Callisto::Event& e) override
	{
		// CALLISTO_INFO("Event: {0}", e);
	}
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
	return new Sandbox{};
}