#include "CallistoPCH.h"
#include "OrthographicCameraController.h"

#include <Callisto/Core/Input.h>
#include <Callisto/Core/KeyCodes.h>

namespace Callisto
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:
		m_AspectRatio(aspectRatio),
		m_ZoomLevel(1),
		m_Rotation(rotation),
		m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel })
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep timeStep)
	{
		CALLISTO_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(CALLISTO_KEY_W))
			m_CamPos.y += m_CamMoveSpeed * timeStep;
		else if (Input::IsKeyPressed(CALLISTO_KEY_S))
			m_CamPos.y -= m_CamMoveSpeed * timeStep;
		if (Input::IsKeyPressed(CALLISTO_KEY_A))
			m_CamPos.x -= m_CamMoveSpeed * timeStep;
		else if (Input::IsKeyPressed(CALLISTO_KEY_D))
			m_CamPos.x += m_CamMoveSpeed * timeStep;
		
		m_Camera.SetPosition(m_CamPos);

		if(m_Rotation)
		{
			if (Input::IsKeyPressed(CALLISTO_KEY_Q))
				m_CamRot += m_CamRotSpeed * timeStep;
			else if (Input::IsKeyPressed(CALLISTO_KEY_E))
				m_CamRot -= m_CamRotSpeed * timeStep;
			m_Camera.SetRotation(m_CamRot);
		}
	
	}
	bool OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispacther dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(CALLISTO_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CALLISTO_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizeEvent));
		return false;
	}
	bool OrthographicCameraController::OnMouseScrolled(const MouseScrollEvent& e)
	{
		CALLISTO_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetOffsetY() * 0.25f;
		m_ZoomLevel = std::max(0.25f, m_ZoomLevel);

		m_CamMoveSpeed = m_ZoomLevel * 1.5f;

		CalculateView();
		return false;
	}
	bool OrthographicCameraController::OnWindowResizeEvent(const WindowResizeEvent& e)
	{
		CALLISTO_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		CalculateView();
		return false;
	}
	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}
}
