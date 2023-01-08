#include "ExampleLayer2D.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Callisto/Utils/Timer.h>


namespace MyApp
{
	

	ExampleLayer2D::ExampleLayer2D()
		:
		Layer("Example2D"),
		m_CameraController(1280.0f / 720.0f, true), // 1280.0f / 720.0f
		m_ParticleSystem(10000)
	{
	}

	void ExampleLayer2D::OnAttach()
	{
		CALLISTO_PROFILE_FUNCTION();

		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 5.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };

		m_CheckerTexture = Callisto::Texture2D::Create("./Assets/Checkerboard.png");

		m_Textures.push_back(Callisto::Texture2D::Create("./Assets/Image4.png"));
		m_Textures.push_back(Callisto::Texture2D::Create("./Assets/Image5.png"));
		m_Textures.push_back(Callisto::Texture2D::Create("./Assets/Image6.png"));

	}
	void ExampleLayer2D::OnDetach()
	{
		CALLISTO_PROFILE_FUNCTION();
	}

	void ExampleLayer2D::OnImGuiRender()
	{
		auto stats(Callisto::Renderer2D::GetStatistics());
		ImGui::Begin("Statistics");

		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Total Indcies Count: %d", stats.GetTotalIndicesCount());
		ImGui::Text("Total Vertex Count: %d", stats.GetTotalVertexCount());
		
		ImGui::End();
		
	}

	void ExampleLayer2D::OnUpdate(Callisto::TimeStep timeStep)
	{
		CALLISTO_PROFILE_FUNCTION();
		CALLISTO_INFO("FrameRate: {0}fps", 1.0f / timeStep);

		/*
		if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_U))
		{
			cubeRot += 90 * timeStep;
		}
		else if (Callisto::Input::IsKeyPressed(CALLISTO_KEY_I))
		{
			cubeRot -= 90 * timeStep;
		}
		*/
		Callisto::Renderer2D::ResetStatistics();
		{
			CALLISTO_PROFILE_SCOPE("RenderCommand Clear:");
			Callisto::RenderCommand::SetClearColor({ 0.4f, 0.01f, 0.5f, 1.0f }); // good purple
			Callisto::RenderCommand::Clear();
		}

		m_CameraController.OnUpdate(timeStep);
		
		{
			CALLISTO_PROFILE_SCOPE("Render:");
			Callisto::Renderer2D::BeginScene(m_CameraController.GetCamera());

			Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.0f), m_CheckerTexture, glm::vec2(10.0f), glm::vec4(0.5f, 0.5f, 1.0f, 1.0f));


			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec2(-0.5f, 0.0f), glm::vec2(0.8f, 0.8f), glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.75f), glm::vec4(0.2f, 0.8f, 0.2f, 1.0f) );

			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec2(-1.5f, 0.0f), glm::vec2(0.5f, 0.5f), m_Textures[0], glm::vec2(4.0f), glm::vec4(1.0f));
			//Callisto::Renderer2D::DrawRotatedQuadFilled(glm::vec2( 0.0f, 0.0f), glm::vec2(0.5f, 0.5f), 45.0f, m_Textures[1], glm::vec2(1.0f), glm::vec4(1.0f));
			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec2(1.5f, 0.5f), glm::vec2(0.25f, 0.25f), m_Textures[2], glm::vec2(1.0f), glm::vec4(1.0f));

			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec2(0.0f, 0.5f), glm::vec2(0.25f, 0.25f), m_Textures[2], glm::vec2(2.0f), glm::vec4(1.0f));
			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec2(0.0f, -0.5f), glm::vec2(0.25f, 0.25f), m_Textures[2], glm::vec2(0.5f), glm::vec4(1.0f));


			//Callisto::Renderer2D::DrawRotatedQuadFilled(glm::vec2(3.0f, 3.0f), glm::vec2(0.5f), cubeRot, glm::vec4(0.8f, 0.4f, 0.4f, 1.0f));
			//Callisto::Renderer2D::DrawRotatedQuadFilled(glm::vec2(3.0f, 3.5f), glm::vec2(0.2f), cubeRot, glm::vec4(0.4f, 0.8f, 0.4f, 1.0f));
			//Callisto::Renderer2D::DrawRotatedQuadFilled(glm::vec2(-3.7f), glm::vec2(0.7f), cubeRot, glm::vec4(0.4f, 0.4f, 0.8f, 1.0f));

			Callisto::Renderer2D::EndScene();
		}

		if (Callisto::Input::IsMouseButtonPressed(CALLISTO_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Callisto::Input::GetMousePosition();
			auto width = Callisto::Application::Get().GetWindow().GetWidth();
			auto height = Callisto::Application::Get().GetWindow().GetHeight();
			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 20; i++)
				m_ParticleSystem.Emit(m_Particle);
		}
		m_ParticleSystem.OnUpdate(timeStep);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());


	}

	void ExampleLayer2D::OnEvent(Callisto::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}