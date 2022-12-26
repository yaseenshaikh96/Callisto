#pragma once

#include <Callisto/Core.h>
#include <Callisto/Events/Event.h>
#include <Callisto/Events/ApplicationEvent.h>
#include <Callisto/Events/KeyEvent.h>
#include <Callisto/Events/MouseEvent.h>

namespace Callisto{
	
	class CALLISTO_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}
