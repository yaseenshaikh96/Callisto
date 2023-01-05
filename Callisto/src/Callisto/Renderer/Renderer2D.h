#pragma once

#include <Callisto/Renderer/Camera.h>
#include <Callisto/Renderer/Texture.h>

namespace Callisto
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4 color = glm::vec4(0.0f));
		static void DrawQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4 color = glm::vec4(0.0f));

		static void DrawQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);
		static void DrawQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);

	};
}