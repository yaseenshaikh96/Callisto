#include "CallistoPCH.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Callisto
{
	SceneCamera::SceneCamera()
	{
		ReCalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicFar = farClip;
		m_OrthographicNear = nearClip;

		ReCalculateProjection();
	}

	void SceneCamera::SetViewPortSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		ReCalculateProjection();
	}

	void SceneCamera::ReCalculateProjection()
	{

		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}