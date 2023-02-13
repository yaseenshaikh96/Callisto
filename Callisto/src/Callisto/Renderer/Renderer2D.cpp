#include "CallistoPCH.h"
#include "Renderer2D.h"

#include "Callisto/Renderer/RenderCommand.h"
#include "Callisto/Renderer/VertexArray.h"
#include "Callisto/Renderer/Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Callisto
{
	const uint32_t Renderer2D::MAX_TEXTURE_SLOTS = 16;
	const uint32_t Renderer2D::MAX_QUAD_COUNT_PER_DRAW = 5000;
	const uint32_t Renderer2D::MAX_VERTICES_COUNT_PER_DRAW = MAX_QUAD_COUNT_PER_DRAW * 4;
	const uint32_t Renderer2D::MAX_INDICES_COUNT_PER_DRAW = MAX_QUAD_COUNT_PER_DRAW * 6;

	struct Renderer2DData
	{
		uint32_t QuadIndexCount = 0;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadTextureShader;
		Ref<Texture2D> WhiteTexture;

		Renderer2D::QuadVertex* QuadVertexBufferBase = nullptr;
		Renderer2D::QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, Renderer2D::MAX_TEXTURE_SLOTS> TextureSlots;
		uint32_t TextureSlotSIndex = 1; // 0 is whitetexture

		glm::vec4 QuadVertexPositions[4];


		Renderer2D:: Statistics stats;
	};
	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		CALLISTO_PROFILE_FUNCTION();


		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexBuffer = VertexBuffer::Create(MAX_VERTICES_COUNT_PER_DRAW * sizeof(QuadVertex) );
		BufferLayout quadLayout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float2, "a_TexScale"}
		};
		s_Data.QuadVertexBuffer->SetLayout(quadLayout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[MAX_VERTICES_COUNT_PER_DRAW];
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		uint32_t* quadIndices = new uint32_t[MAX_INDICES_COUNT_PER_DRAW];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < MAX_INDICES_COUNT_PER_DRAW; i+=6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			
			offset += 4;
		}
	
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, MAX_INDICES_COUNT_PER_DRAW);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.WhiteTexture->SetData((void*)&whiteTextureData, sizeof(whiteTextureData));

		s_Data.QuadTextureShader = Shader::Create("./Assets/Shaders/Texture.glsl");
		s_Data.QuadTextureShader->Bind();

		int32_t samplers[MAX_TEXTURE_SLOTS];
		for (int32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}
		s_Data.QuadTextureShader->SetIntArray("u_Textures", samplers, MAX_TEXTURE_SLOTS);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);
	}

	void Renderer2D::Shutdown()
	{
		CALLISTO_PROFILE_FUNCTION();
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CALLISTO_PROFILE_FUNCTION();

		s_Data.TextureSlotSIndex = 1;

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.QuadTextureShader->Bind();
		s_Data.QuadTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer2D::BeginScene(const Camera& camera, glm::mat4 transform)
	{
		s_Data.TextureSlotSIndex = 1;

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_Data.QuadTextureShader->Bind();
		s_Data.QuadTextureShader->SetMat4("u_ViewProjection", viewProjection);
	}

	void Renderer2D::BeginScene(const EditorCamera& editorCamera)
	{
		s_Data.TextureSlotSIndex = 1;

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.QuadTextureShader->Bind();
		s_Data.QuadTextureShader->SetMat4("u_ViewProjection", editorCamera.GetViewProjection());
	}

	void Renderer2D::EndScene()
	{
		CALLISTO_PROFILE_FUNCTION();
#pragma warning(push)
#pragma warning(disable: 4244)
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
#pragma warning(pop)
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// bind Textures
		for (uint32_t i = 0; i < s_Data.TextureSlotSIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.TextureSlotSIndex = 1;
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	/****************************************************************************************************************************/
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotInRad, glm::vec4 color)
	{
		DrawRotatedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, rotInRad, color);
	}
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotInRad, glm::vec4 color)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotInRad, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotInRad, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		DrawRotatedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, rotInRad, texture, texScale, tintColor);
	}
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotInRad, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotInRad, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, texScale, tintColor);
	}
	/****************************************************/
	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec2& position, const glm::vec2& size, glm::vec4 color)
	{
		DrawAxisAlignedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, color);
	}
	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec3& position, const glm::vec2& size, glm::vec4 color)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		DrawAxisAlignedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, texture, texScale, tintColor);
	}
	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, texScale, tintColor);
	}

	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		DrawAxisAlignedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, subTexture, texScale, tintColor);

	}
	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, texScale, tintColor);

	}
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotInRad, const Ref<SubTexture2D>& subTexture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		DrawRotatedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, rotInRad, subTexture, texScale, tintColor);
	}
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotInRad, const Ref<SubTexture2D>& subTexture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotInRad, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, texScale, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		CALLISTO_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2D::MAX_INDICES_COUNT_PER_DRAW)
		{
			FlushAndReset();
		}

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] =
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};
		const glm::vec2 texScale(1.0f, 1.0f);

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TexScale = texScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		CALLISTO_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= MAX_INDICES_COUNT_PER_DRAW)
		{
			FlushAndReset();
		}

		const glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] =
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotSIndex; i++)
		{
			if (*(s_Data.TextureSlots[i].get()) == *(texture.get()))
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotSIndex;
			s_Data.TextureSlots[s_Data.TextureSlotSIndex] = texture;
			s_Data.TextureSlotSIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TexScale = texScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{

		if (s_Data.QuadIndexCount >= MAX_INDICES_COUNT_PER_DRAW)
		{
			FlushAndReset();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotSIndex; i++)
		{
			if (*(s_Data.TextureSlots[i].get()) == *(subTexture->GetTexture().get()))
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotSIndex;
			s_Data.TextureSlots[s_Data.TextureSlotSIndex] = subTexture->GetTexture();
			s_Data.TextureSlotSIndex++;
		}

		const glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TexScale = texScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.stats.QuadCount++;
	}

	void Renderer2D::ResetStatistics()
	{
		s_Data.stats.DrawCalls = 0;
		s_Data.stats.QuadCount = 0;
	}
	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_Data.stats;
	}
}