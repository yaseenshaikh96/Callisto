#include "ExampleLayer2D.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Callisto/Utils/Timer.h>

namespace MyApp
{
	ExampleLayer2D::ProfilerResult::ProfilerResult(const char* name, float duration)
		:Name(name), Timer(duration)
	{}

#define PROFILER_SCOPED(name) Callisto::Timer timer##__LINE__(name, [&](ProfilerResult profileResult) { m_ProfilerResults.push_back(profileResult); })

	ExampleLayer2D::ExampleLayer2D()
		:
		Layer("Example2D"),
		m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void ExampleLayer2D::OnAttach()
	{
		m_CheckerTexture = Callisto::Texture2D::Create("./Assets/Checkerboard.png");

	}

	void ExampleLayer2D::OnImGuiRender()
	{
		
		ImGui::Begin("Timer");
		
		for (auto& result : m_ProfilerResults)
		{
			char label[50];
			strcpy(label, result.Name);
			strcat(label, "  %.3fms");
			ImGui::Text(label, result.Timer);
		}
		ImGui::End();
		m_ProfilerResults.clear();
		
	}

	void ExampleLayer2D::OnUpdate(Callisto::TimeStep timeStep)
	{

		if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_U))
		{
			CALLISTO_INFO("u PRESSED : {0}", cubeRot);
			cubeRot += 90 * timeStep;
		}
		else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_I))
		{
			CALLISTO_INFO("i PRESSED");
			cubeRot -= 90 * timeStep;
		}

		{
			PROFILER_SCOPED("Render Clear:");
			Callisto::RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
			Callisto::RenderCommand::Clear();
		}

		{
			PROFILER_SCOPED("Camera:");
			m_CameraController.OnUpdate(timeStep);
		}

		
		{
			PROFILER_SCOPED("Render:");
			Callisto::Renderer2D::BeginScene(m_CameraController.GetCamera());

			Callisto::Renderer2D::DrawQuadFilled(glm::vec2(0.0f, 0.0f), glm::vec2(0.5f), cubeRot, glm::vec4(0.8f, 0.4f, 0.4f, 1.0f));
			Callisto::Renderer2D::DrawQuadFilled(glm::vec2(0.0f, 0.5f), glm::vec2(0.2f), cubeRot, glm::vec4(0.4f, 0.8f, 0.4f, 1.0f));
			Callisto::Renderer2D::DrawQuadFilled(glm::vec2(-0.7f), glm::vec2(0.7f), cubeRot, glm::vec4(0.4f, 0.4f, 0.8f, 1.0f));

			Callisto::Renderer2D::DrawQuadFilled(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.f), 0, m_CheckerTexture);

			Callisto::Renderer2D::EndScene();
		}

	}

	void ExampleLayer2D::OnEvent(Callisto::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}