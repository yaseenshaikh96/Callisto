#pragma once

#include <glm/glm.hpp>
#include <string>

#include <Callisto/Renderer/Texture.h>

namespace Callisto
{
	class SubTexture2D
	{
	public:
		static Ref<SubTexture2D> CreateFromCoords
		(
			const Ref<Texture2D>& texture, 
			const glm::vec2& coord,									// in pixel, bottom-left 
			const glm::vec2& cellSize,								// in pixel, size of one cell,
			const glm::vec2& spriteSize = glm::vec2(1.0f, 1.0f)		// in integers, how many cells
		);
		
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		const Ref<Texture2D>& GetTexture() const { return m_Texture; }

	private:
		const Ref<Texture2D>& m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}