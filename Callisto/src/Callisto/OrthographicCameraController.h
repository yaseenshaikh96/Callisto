#pragma once

#include <Callisto/Renderer/Camera.h>
#include <Callisto/Core/TimeStep.h>
#include <Callisto/Events/MouseEvent.h>
#include <Callisto/Events/ApplicationEvent.h>

namespace Callisto
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(TimeStep timeStep);
		bool OnEvent(Event& e);
	
	private:
		bool OnMouseScrolled(const MouseScrollEvent& e);
		bool OnWindowResizeEvent(const WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		bool m_Rotation;
		
		float m_CamRot = 0;
		glm::vec3 m_CamPos = glm::vec3(0.0f);

		float m_CamMoveSpeed = 3.0f;
		float m_CamRotSpeed = 90;

		OrthographicCamera m_Camera;
	};

}