#include "CallistoPCH.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Callisto
{
	static GLenum ShaderDataTypeToOpenGlType(ShaderDataType type)
	{
		switch (type)
		{
		case Callisto::ShaderDataType::None:	return GL_FLOAT;
		case Callisto::ShaderDataType::Float:	return GL_FLOAT;
		case Callisto::ShaderDataType::Float2:	return GL_FLOAT;
		case Callisto::ShaderDataType::Float3:	return GL_FLOAT;
		case Callisto::ShaderDataType::Float4:	return GL_FLOAT;
		case Callisto::ShaderDataType::Mat3:	return GL_FLOAT;
		case Callisto::ShaderDataType::Mat4:	return GL_FLOAT;
		case Callisto::ShaderDataType::Int:		return GL_INT;
		case Callisto::ShaderDataType::Int2:	return GL_INT;
		case Callisto::ShaderDataType::Int3:	return GL_INT;
		case Callisto::ShaderDataType::Int4:	return GL_INT;
		case Callisto::ShaderDataType::Bool:	return GL_BOOL;
		}
		CALLISTO_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		CALLISTO_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGlType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}