#pragma once

#include <vector>

#include "Callisto.h"

namespace Callisto
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		// colors
		RGBA_8,

		// depths
		DEPTH_24_STENCIL_8,

		// defaults
		Depth = DEPTH_24_STENCIL_8,
		Color = RGBA_8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat textureFormat)
			:
			TextureFormat(textureFormat)
		{}
		
		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			:
			Attachments(attachments)
		{}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t SampleCount;
		FrameBufferAttachmentSpecification Attachments;
		bool SwapChainTarget = false;
	};


	class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specs);
		virtual ~FrameBuffer() = default;

		virtual const FrameBufferSpecification& GetSpecifications() const = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const = 0;
	};
}