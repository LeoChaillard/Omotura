#pragma once

#include <glad/glad.h>

namespace Omotura
{
	class FrameBuffer
	{
	public:
		GLuint m_FBO;
		GLuint m_RBO;

	public:
		FrameBuffer();
		~FrameBuffer() = default;

		void Bind(GLenum _textureUnit);
		void Unbind();
		void Clear();

	private:
		void Init();
	};
}