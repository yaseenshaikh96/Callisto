#pragma once

#include <Callisto/Core/Core.h>
#include <Callisto/Events/Event.h>
#include <Callisto/Core/Layer.h>

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
	};
}


