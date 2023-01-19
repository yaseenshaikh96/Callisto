#pragma once

#include "Callisto/Renderer/Camera.h"

namespace Callisto
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
	
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewPortSize(uint32_t width, uint32_t height);

		float GetOrthographicsSize() const { return m_OrthographicSize; }
		void SetOrthographicsSize(float size) { m_OrthographicSize = size; ReCalculateProjection(); }

	private:
		void ReCalculateProjection();

	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;
		float m_AspectRatio = 1.7f;

	};
}