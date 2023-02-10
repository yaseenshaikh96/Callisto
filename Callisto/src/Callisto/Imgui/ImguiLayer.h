#pragma once

#include <Callisto/Core/Core.h>
#include <Callisto/Events/Event.h>
#include <Callisto/Core/Layer.h>

namespace Callisto
{
	class CALLISTO_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnEvent(Event& e) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void SetBlockImGuiEvent(bool block) { m_BlockImGuiEvent = block; }

		void Begin();
		void End();
	
	private:
		void SetImGuiDarkTheme();

	private:
		bool m_BlockImGuiEvent = true;
	};
}


