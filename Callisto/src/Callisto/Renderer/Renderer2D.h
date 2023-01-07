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

		static void Flush();
		
		
		static void DrawAxisAlignedQuadFilled(const glm::vec2& position, const glm::vec2& size, glm::vec4 color = glm::vec4(0.0f));
		static void DrawAxisAlignedQuadFilled(const glm::vec3& position, const glm::vec2& size, glm::vec4 color = glm::vec4(0.0f));

		static void DrawAxisAlignedQuadFilled(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor);
		static void DrawAxisAlignedQuadFilled(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor);

		static void DrawRotatedQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4 color = glm::vec4(0.0f));
		static void DrawRotatedQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4 color = glm::vec4(0.0f));

		static void DrawRotatedQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor);
		static void DrawRotatedQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor);

	public:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			// normal, texture id, etc
		};

		static const uint32_t MAX_TEXTURE_SLOTS;
	private:
		static const uint32_t MAX_QUAD_COUNT_PER_DRAW;
		static const uint32_t MAX_VERTICES_COUNT_PER_DRAW;
		static const uint32_t MAX_INDICES_COUNT_PER_DRAW;
	};
}