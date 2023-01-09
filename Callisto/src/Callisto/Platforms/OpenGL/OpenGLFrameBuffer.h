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

		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachment; }

		virtual const FrameBufferSpecification& GetSpecifications() const override { return m_Specs; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FrameBufferSpecification& m_Specs;
	};
}