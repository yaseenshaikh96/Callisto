#include "CallistoPCH.h"
#include "SubTexture2D.h"

namespace Callisto
{

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{

		float sheetWidthPx = (float)texture->GetWidth();
		float  sheetheightPx = (float)texture->GetHeight();
		return CreateRef<SubTexture2D>
		(
			texture,
			glm::vec2((coord.x * cellSize.x) / sheetWidthPx                 , (coord.y * cellSize.y) / sheetheightPx), // min
			glm::vec2(((coord.x + spriteSize.x) * cellSize.x) / sheetWidthPx, ((coord.y + spriteSize.y) * cellSize.y) / sheetheightPx)  // max
		);
	}

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = glm::vec2(min.x, min.y);
		m_TexCoords[1] = glm::vec2(max.x, min.y);
		m_TexCoords[2] = glm::vec2(max.x, max.y);
		m_TexCoords[3] = glm::vec2(min.x, max.y);
	}

}