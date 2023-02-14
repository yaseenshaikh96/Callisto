#include "CallistoPCH.h"
#include "OpenGLFrameBuffer.h"
#include <glad/glad.h>

#pragma warning(disable : 4267)

namespace Callisto
{
	static bool IsDepthFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::DEPTH_24_STENCIL_8:
			return true;
		default:
			return false;
		}
		return false;
	}

	static GLenum TextureTarget(bool multiSample)
	{
		return multiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void BindTexture(bool multiSample, uint32_t id)
	{
		glBindTexture(TextureTarget(multiSample), id);
	}

	static void CreateTextures(bool multiSample, uint32_t* outTextureIDs, size_t count)
	{
		glGenTextures(count, outTextureIDs);
	}

	static void AttachColorTexture(uint32_t id, uint32_t sampleCount, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, uint32_t index)
	{
		bool multiSample = sampleCount > 1;
		if (multiSample)
		{
			glTexImage2DMultisample(id, sampleCount, format, width, height, GL_FALSE);
		}
		else
		{
			BindTexture(multiSample, id);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSample), id, 0);
		}
	}

	static void AttachDepthTexture(uint32_t id, uint32_t sampleCount, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{

		bool multiSample = sampleCount > 1;
		if (multiSample)
		{
			glTexImage2DMultisample(id, sampleCount, format, width, height, GL_FALSE);
		}
		else
		{
			BindTexture(multiSample, id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSample), id, 0);
		}
	}


	/******************************************************************************************************************************************************/
	/* OpenGLFrameBuffer */
	/******************************************************************************************************************************************************/
	OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferSpecification specs)
		: m_Specs(specs)
	{

		for (auto spec : m_Specs.Attachments.Attachments)
		{
			if (!IsDepthFormat(spec.TextureFormat))
			{
				m_ColorAttachmentSpecs.push_back(spec);
			}
			else
			{
				m_DepthAttachmentSpecs = spec;
			}
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
		glDeleteTextures(1, &m_DepthAttachmentID);

		m_ColorAttachmentIDs.clear();
		m_DepthAttachmentID = 0;
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specs.Width = width;
		m_Specs.Height = height;
		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID != 0)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
			glDeleteTextures(1, &m_DepthAttachmentID);

			m_ColorAttachmentIDs.clear();
			m_DepthAttachmentID = 0;
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multiSample = m_Specs.SampleCount > 1;
		//Attachments
		if (m_ColorAttachmentSpecs.size())
		{
			m_ColorAttachmentIDs.resize(m_ColorAttachmentSpecs.size());
			CreateTextures(multiSample, m_ColorAttachmentIDs.data(), m_ColorAttachmentIDs.size());

			for (size_t i = 0; i < m_ColorAttachmentIDs.size(); i++)
			{
				BindTexture(multiSample, m_ColorAttachmentIDs[i]);
				switch (m_ColorAttachmentSpecs[i].TextureFormat)
				{
				case FrameBufferTextureFormat::RGBA_8:
					AttachColorTexture(m_ColorAttachmentIDs[i], m_Specs.SampleCount, GL_RGBA8, GL_RGBA, m_Specs.Width, m_Specs.Height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					AttachColorTexture(m_ColorAttachmentIDs[i], m_Specs.SampleCount, GL_R32I, GL_RED_INTEGER, m_Specs.Width, m_Specs.Height, i);
					break;
				default:
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecs.TextureFormat != FrameBufferTextureFormat::None)
		{
			CreateTextures(multiSample, &m_DepthAttachmentID, 1);
			BindTexture(multiSample, m_DepthAttachmentID);
			switch (m_DepthAttachmentSpecs.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH_24_STENCIL_8:
				AttachDepthTexture(m_DepthAttachmentID, m_Specs.SampleCount, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
				break;
			default:
				break;
			}
		}

		if (m_ColorAttachmentIDs.size() > 1)
		{
			CALLISTO_CORE_ASSERT(m_ColorAttachmentIDs.size() <= 4, "Only four color atachments are supported");

			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachmentIDs.size(), buffers);
		}
		else if (m_ColorAttachmentIDs.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		/*
		
		glGenTextures(1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specs.Width, m_Specs.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
		
		glGenTextures(1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specs.Width, m_Specs.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
		*/

		CALLISTO_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, " FrameBuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specs.Width, m_Specs.Height);
	}
	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		CALLISTO_CORE_ASSERT(attachmentIndex < m_ColorAttachmentIDs.size(), "IndexOutOfBound");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
}