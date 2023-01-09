#pragma once

#include <Callisto.h>

namespace MyApp
{
	class ExampleLayer2DSpriteSheets : public Callisto::Layer
	{
	public:
		ExampleLayer2DSpriteSheets();

		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Callisto::TimeStep timeStep) override;
		virtual void OnEvent(Callisto::Event& e) override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

	private:
		uint32_t m_MapWidth, m_MapHeight;
		const char* m_MapTiles;

		Callisto::Ref<Callisto::SubTexture2D> m_StairsTexture;
		Callisto::Ref<Callisto::SubTexture2D> m_TreeTexture;

		std::unordered_map<char, Callisto::Ref<Callisto::SubTexture2D>> m_MapTileTextures;

		std::vector<Callisto::Ref<Callisto::Texture2D>> m_Textures;
	
		Callisto::Ref<Callisto::Texture2D> m_SheetTexture;
		
		Callisto::Ref<Callisto::Texture2D> m_CheckerTexture;
		Callisto::OrthographicCameraController m_CameraController;
		float cubeRot = 0.0f;
	};
}