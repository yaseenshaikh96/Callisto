#pragma once

#include <Callisto/Renderer/FrameBuffer.h>

namespace Callisto
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(FrameBufferSpecification specs);

		virtual ~OpenGLFrameBuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachment; }

		virtual const FrameBufferSpecification& GetSpecifications() const override { return m_Specs; }

	private:
		FrameBufferSpecification m_Specs;
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
	};
}