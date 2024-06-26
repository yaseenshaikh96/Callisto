#include "ExampleLayer2DSpriteSheets.h"
//

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Callisto/Utils/Timer.h>


namespace MyApp
{


	ExampleLayer2DSpriteSheets::ExampleLayer2DSpriteSheets()
		:
		Layer("Example2D"),
		m_CameraController(1280.0f / 720.0f, true) // 1280.0f / 720.0f
	{

		m_MapTiles =
			"wwwwwwwwwwwwwwwwwwwwwwww"
			"wwwwwwwwdddddwwwwww  www"
			"wwwwwwdddgggdddwwwwwwwww"
			"wwwwdddgggggggdddwwwwwww"
			"wwwddggggggggwggdddwwwww"
			"wwddggggggggwwwwggdddwww"
			"wwddggggggggggwwwgggddww"
			"wwwddgggggggggggggggddww"
			"wwwddggggggggggggggddwww"
			"wwwwddgggggggggggggddwww"
			"wwwwwdddgggggggggdddwwww"
			"wwwwwwwdddgggggdddwwwwww"
			"wwwwwwwwwwddddddwwwwwwww"
			"wwwwwwwwwwwdddwwwwwwwwww"
			"wwwwwwwwwwwwwwwwwwwwwwww"
		;
		m_MapWidth = 24;
		m_MapHeight = (uint32_t)(strlen(m_MapTiles) / m_MapWidth);
	}

	void ExampleLayer2DSpriteSheets::OnAttach()
	{
		CALLISTO_PROFILE_FUNCTION();

		m_CameraController.SetZoomLevel(5.0f);

		m_CheckerTexture = Callisto::Texture2D::Create("./Assets/Checkerboard.png");

		m_Textures.push_back(Callisto::Texture2D::Create("./Assets/Image4.png"));
		m_Textures.push_back(Callisto::Texture2D::Create("./Assets/Image5.png"));
		m_Textures.push_back(Callisto::Texture2D::Create("./Assets/Image6.png"));

		m_SheetTexture = Callisto::Texture2D::Create("./Assets/Game/RPGpack_sheet_20x13-128x128.png");

		m_StairsTexture = Callisto::SubTexture2D::CreateFromCoords(m_SheetTexture, glm::vec2(6.0f, 3.0f), glm::vec2(128.0f, 128.0f), glm::vec2(1.0f,1.0f));
		m_TreeTexture = Callisto::SubTexture2D::CreateFromCoords(m_SheetTexture, glm::vec2(2.0f, 1.0f), glm::vec2(128.0f, 128.0f), glm::vec2(1.0f, 2.0f));
	
		m_MapTileTextures['g'] = Callisto::SubTexture2D::CreateFromCoords(m_SheetTexture, glm::vec2(1.0f, 11.0f), glm::vec2(128.0f, 128.0f), glm::vec2(1.0f, 1.0f));
		m_MapTileTextures['d'] = Callisto::SubTexture2D::CreateFromCoords(m_SheetTexture, glm::vec2(6.0f, 11.0f), glm::vec2(128.0f, 128.0f), glm::vec2(1.0f, 1.0f));
		m_MapTileTextures['w'] = Callisto::SubTexture2D::CreateFromCoords(m_SheetTexture, glm::vec2(11.0f, 11.0f), glm::vec2(128.0f, 128.0f), glm::vec2(1.0f, 1.0f));
	
	}
	void ExampleLayer2DSpriteSheets::OnDetach()
	{
		CALLISTO_PROFILE_FUNCTION();
	}

	void ExampleLayer2DSpriteSheets::OnImGuiRender()
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

	void ExampleLayer2DSpriteSheets::OnUpdate(Callisto::TimeStep timeStep)
	{
		CALLISTO_PROFILE_FUNCTION();
		CALLISTO_INFO("FrameRate: {0}fps", 1.0f / timeStep);

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
				
			// xStart = 2 * 128 , yStart = 3 * 128

			Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.0f), m_CheckerTexture, glm::vec2(10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(0.0f, 0.0f, 0.1f), glm::vec2(1.0f), m_SpriteTexture, glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(0.0f, 0.0f, 0.1f), glm::vec2(1.0f), m_SpriteTexture, glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			//Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(1.0f, 0.0f, 0.1f), glm::vec2(1.0f, 2.0f), m_TreeTexture, glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			
			
			for (uint32_t y = 0; y < m_MapHeight; y++)
			{
				for (uint32_t x = 0; x < m_MapWidth; x++)
				{
					char tileType = m_MapTiles[(y * m_MapWidth) + x];
					Callisto::Ref<Callisto::SubTexture2D> texture;
					if (m_MapTileTextures.find(tileType) != m_MapTileTextures.end())
						texture = m_MapTileTextures[tileType];
					else
						texture = m_StairsTexture;

					Callisto::Renderer2D::DrawAxisAlignedQuadFilled(glm::vec3(x - (m_MapWidth / 2.0f), -(y - (m_MapHeight / 2.0f)), 0.1f), glm::vec2(1.0f), texture, glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					
				
				}
			}
			


			Callisto::Renderer2D::EndScene();

		}

	}

	void ExampleLayer2DSpriteSheets::OnEvent(Callisto::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}