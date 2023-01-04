#pragma once

#include <Callisto/Core/Core.h>
#include <Callisto/Events/Event.h>
#include <Callisto/Events/ApplicationEvent.h>
#include <Callisto/Events/KeyEvent.h>
#include <Callisto/Events/MouseEvent.h>

#include <Callisto/Core/TimeStep.h>

namespace Callisto{
	
	class CALLISTO_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep timeStep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}
