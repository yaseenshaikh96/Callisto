#include "CallistoPCH.h"
#include "Renderer2D.h"

#include <Callisto/Renderer/RenderCommand.h>
#include <Callisto/Renderer/VertexArray.h>
#include <Callisto/Renderer/Shader.h>

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
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Int, "a_TexIndex"}
		};
		s_Data.QuadVertexBuffer->SetLayout(quadLayout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[MAX_VERTICES_COUNT_PER_DRAW];

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
	void Renderer2D::EndScene()
	{
		CALLISTO_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
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
	}


	/****************************************************************************************************************************/
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4 color)
	{
		DrawRotatedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, rotation, color);
	}
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4 color)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), 
				glm::radians(rotation),
				glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f) , glm::vec3(size.x, size.y, 1.0f));

		s_Data.WhiteTexture->Bind();
		
		s_Data.QuadTextureShader->SetFloat4("u_Color", color);
		s_Data.QuadTextureShader->SetFloat2("u_TexScale", glm::vec2(1.0f));
		s_Data.QuadTextureShader->SetMat4("u_Transform", transform);
		
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		DrawRotatedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, rotation, texture, texScale, tintColor);
	}
	void Renderer2D::DrawRotatedQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		CALLISTO_PROFILE_FUNCTION();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f),
				glm::radians(rotation),
				glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		texture->Bind();
		
		s_Data.QuadTextureShader->SetFloat4("u_Color", tintColor); // Tint
		s_Data.QuadTextureShader->SetFloat2("u_TexScale", texScale);
		s_Data.QuadTextureShader->SetMat4("u_Transform", transform);
		
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
	/****************************************************/
	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec2& position, const glm::vec2& size, glm::vec4 color)
	{
		DrawAxisAlignedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, color);
	}
	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec3& position, const glm::vec2& size, glm::vec4 color)
	{
		CALLISTO_PROFILE_FUNCTION();
		
		const float texIndex = 0.0f; // white texture

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;
		  
		s_Data.QuadVertexBufferPtr->Position = glm::vec3(position.x + size.x, position.y, position.z);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;
		  
		s_Data.QuadVertexBufferPtr->Position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;
		  
		s_Data.QuadVertexBufferPtr->Position = glm::vec3(position.x, position.y + size.y, position.z);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		DrawAxisAlignedQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, texture, texScale, tintColor);
	}
	void Renderer2D::DrawAxisAlignedQuadFilled(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& texScale, const glm::vec4& tintColor)
	{
		CALLISTO_PROFILE_FUNCTION();

		const glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


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
		/*
		bool textureFound = false;
		float textureIndex = 0;
		for(uint32_t i=1; i < s_Data.TextureSlotSIndex; i++)
		{
			if (*(s_Data.TextureSlots[i].get()) == *(texture.get()))
			{
				textureFound = true;
				textureIndex = (float)i;
				break;
			}
		}
		if(!textureFound)
		{
			s_Data.TextureSlots[s_Data.TextureSlotSIndex] = texture;
			textureIndex = (float)s_Data.TextureSlotSIndex;
			s_Data.TextureSlotSIndex++;
		}
		*/
		
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = whiteColor;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = glm::vec3(position.x + size.x, position.y, position.z);
		s_Data.QuadVertexBufferPtr->Color = whiteColor;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
		s_Data.QuadVertexBufferPtr->Color = whiteColor;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = glm::vec3(position.x, position.y + size.y, position.z);
		s_Data.QuadVertexBufferPtr->Color = whiteColor;
		s_Data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}
}