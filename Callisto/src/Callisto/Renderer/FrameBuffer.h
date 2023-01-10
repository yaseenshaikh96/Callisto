#pragma once

#include <Callisto.h>
namespace Callisto
{
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t SampleCount;

		bool SwapChainTarget = false;
	};


	class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specs);

		virtual const FrameBufferSpecification& GetSpecifications() const = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentID() const = 0;
	};
}