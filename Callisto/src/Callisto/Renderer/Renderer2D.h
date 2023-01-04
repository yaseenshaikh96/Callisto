#pragma once

#include <Callisto/Renderer/Camera.h>

namespace Callisto
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation = 0, glm::vec4 color = glm::vec4(0.0f));
		static void DrawQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation = 0, glm::vec4 color = glm::vec4(0.0f));

	};
}