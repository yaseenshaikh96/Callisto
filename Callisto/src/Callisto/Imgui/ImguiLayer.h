#pragma once

#include <Callisto/Core.h>
#include <Callisto/Events/Event.h>
#include <Callisto/Layer.h>



namespace Callisto
{
	class CALLISTO_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time;
	};
}


