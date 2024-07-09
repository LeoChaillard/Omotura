#include "FrameBuffer.h"

#include "../BackEnd/BackEnd.h"

#include <iostream>

namespace Omotura
{
	FrameBuffer::FrameBuffer()
	{
		Init();
	}

	void FrameBuffer::Init()
	{
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Generate color texture
		GLuint textureColorBuffer;
		glGenTextures(1, &textureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Generate render buffer for depth and stencil testing
		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// Make attachments
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
		glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		// Check if framebuffer is valid
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not valid!" << std::endl;
		}

		// Unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Bind(GLenum _textureUnit)
	{
		glActiveTexture(_textureUnit);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Clear()
	{
		glDeleteFramebuffers(1, &m_FBO);
	}
}