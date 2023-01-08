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

		Callisto::Ref<Callisto::SubTexture2D> m_SpriteTexture;

		std::vector<Callisto::Ref<Callisto::Texture2D>> m_Textures;
	
		Callisto::Ref<Callisto::Texture2D> m_SheetTexture;
		
		Callisto::Ref<Callisto::Texture2D> m_CheckerTexture;
		Callisto::OrthographicCameraController m_CameraController;
		float cubeRot = 0.0f;
	};
}