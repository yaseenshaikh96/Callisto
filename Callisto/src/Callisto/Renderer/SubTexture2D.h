#pragma once

#include <glm/glm.hpp>
#include <string>

#include <Callisto/Renderer/Texture.h>

namespace Callisto
{
	class SubTexture2D
	{
	public:
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& spriteSize);
		
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		const Ref<Texture2D>& GetTexture() const { return m_Texture; }

	private:
		const Ref<Texture2D>& m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}