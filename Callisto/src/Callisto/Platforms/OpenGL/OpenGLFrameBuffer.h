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

		virtual void ClearColorAttacment(uint32_t attachmentIndex, const void* value) override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override 
		{  
			CALLISTO_CORE_ASSERT(index < m_ColorAttachmentIDs.size(), "IndexOutOfBound!");
			return m_ColorAttachmentIDs[index]; 
		}

		virtual const FrameBufferSpecification& GetSpecifications() const override { return m_Specs; }

	private:
		FrameBufferSpecification m_Specs;
		uint32_t m_RendererID = 0;

		std::vector<uint32_t> m_ColorAttachmentIDs;
		uint32_t m_DepthAttachmentID;
		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecs;
		FrameBufferTextureSpecification m_DepthAttachmentSpecs;
	};
}