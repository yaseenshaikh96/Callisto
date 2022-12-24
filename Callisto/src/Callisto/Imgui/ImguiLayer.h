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

		void OnUpdate() override;
		void OnEvent(Event& e) override;
		void OnAttach() override;
		void OnDetach() override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseScrollEvent(MouseScrollEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		bool OnWindowCloseEvent(WindowCloseEvent& e);
		bool OnWindowMovedEvent(WindowMovedEvent& e);

	private:
		float m_Time;
	};
}


