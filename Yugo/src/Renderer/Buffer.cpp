#include "pch.h"
#include "Buffer.h"

namespace Yugo
{

	
	 /*Frame Buffer*/

	FrameBuffer::FrameBuffer()
	{
		glGenFramebuffers(1, &m_BufferId);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_BufferId);
	}

	uint32_t FrameBuffer::GetId() const
	{
		return m_BufferId;
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Attach(uint32_t bufferId, AttachmentType type)
	{
		if (type == AttachmentType::TextureBuffer)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferId, 0);
		if (type == AttachmentType::RenderBuffer)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, bufferId);
	}

	void FrameBuffer::AttachMultisampled(uint32_t bufferId, AttachmentType type)
	{
		if (type == AttachmentType::TextureBuffer)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, bufferId, 0);
		if (type == AttachmentType::RenderBuffer)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, bufferId);
	}

	void FrameBuffer::BlitMultisampled(int width, int height, uint32_t bufferId, uint32_t intermediateBufferId)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, bufferId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateBufferId);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}


	/*Render Buffer*/

	RenderBuffer::RenderBuffer()
	{
		glGenRenderbuffers(1, &m_BufferId);
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &m_BufferId);
	}

	uint32_t RenderBuffer::GetId() const
	{
		return m_BufferId;
	}

	void RenderBuffer::Bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_BufferId);
	}

	void RenderBuffer::Unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void RenderBuffer::Storage(int width, int height)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	void RenderBuffer::Storage(int width, int height, int numOfSamples)
	{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, numOfSamples, GL_DEPTH24_STENCIL8, width, height);
	}

	void RenderBuffer::Resize(int width, int height)
	{
		Bind();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		Unbind();
	}

	void RenderBuffer::ResizeMultisampled(int width, int height, int numOfSamples)
	{
		Bind();
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, numOfSamples, GL_DEPTH24_STENCIL8, width, height);
		Unbind();
	}

}