#pragma once

#include "Callisto/Renderer/Camera.h"

namespace Callisto
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType 
		{ 
			Perspective = 0, Orthographic = 1
		};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
	
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewPortSize(uint32_t width, uint32_t height);


		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType projectionType) { m_ProjectionType = projectionType; ReCalculateProjection(); }


		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; ReCalculateProjection(); }
		float GetOrthoGraphicNearClip() const { return m_OrthographicNear; }
		void SetOrthoGraphicNearClip(float OrthographicNear) { m_OrthographicNear = OrthographicNear; ReCalculateProjection(); }
		float GetOrthoGraphicFarClip() const { return  m_OrthographicFar; }
		void SetOrthoGraphicFarClip(float OrthographicFar) { m_OrthographicFar = OrthographicFar; ReCalculateProjection(); }

		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float FOV) { m_PerspectiveFOV = FOV; ReCalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float perspectiveNear) { m_PerspectiveNear = perspectiveNear; ReCalculateProjection(); }
		float GetPerspectiveFarClip() const { return  m_PerspectiveFar; }
		void SetPerspectiveFarClip(float perspectiveFar) { m_PerspectiveFar = perspectiveFar; ReCalculateProjection(); }

	private:
		void ReCalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 1.7f;
	};
}