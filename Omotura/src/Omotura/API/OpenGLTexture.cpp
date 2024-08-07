#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <compressonator/compressonator.h>

namespace Omotura
{
	static GLenum ImageFormatToGLFormat(ImageFormat _format)
	{
		switch (_format)
		{
		case ImageFormat::RED: return GL_RED;
		case ImageFormat::RG: return GL_RG;
		case ImageFormat::RGB: return GL_RGB;
		case ImageFormat::RGBA: return GL_RGBA;
		}
		return GL_RGB;
	}

	static GLenum CompressedImageFormatToGLFormat(ImageFormat _format)
	{
		switch (_format)
		{
			case ImageFormat::COMPRESSED_RGB_DXT1: return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			case ImageFormat::COMPRESSED_RGBA_DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			case ImageFormat::COMPRESSED_RGBA_DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		}
		return CMP_FORMAT_DXT1;
	}

	// OpenGLTexture2D
	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& _specifications, uint8_t* _pData, bool _bCompressed)
		: m_specifications(_specifications), 
		m_imageWidth(_specifications.iWidth), 
		m_imageHeight(_specifications.iHeight),
		m_textureTarget(GL_TEXTURE_2D)
	{
		if (_bCompressed)
		{
			LoadInternalCompressed(_pData);
		}
		else
		{
			LoadInternal(_pData);		
		}
	}

	void OpenGLTexture2D::LoadInternal(const void* _pImageData)
	{
		glGenTextures(1, &m_textureObj);
		glBindTexture(m_textureTarget, m_textureObj);

		GLenum glFormat = ImageFormatToGLFormat(m_specifications.format);
		glTexImage2D(m_textureTarget, 0, glFormat, m_imageWidth, m_imageHeight, 0, glFormat, GL_UNSIGNED_BYTE, _pImageData);

		glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(m_textureTarget);

		glBindTexture(m_textureTarget, 0);
	}

	void OpenGLTexture2D::LoadInternalCompressed(const void* _pImageData)
	{
		glGenTextures(1, &m_textureObj);
		glBindTexture(m_textureTarget, m_textureObj);

		GLenum glFormat = CompressedImageFormatToGLFormat(m_specifications.format);
		glCompressedTexImage2D(m_textureTarget, 0, glFormat, m_specifications.iWidth, m_specifications.iHeight, 0, m_specifications.iDataSize, _pImageData);

		glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 28);
		glGenerateMipmap(m_textureTarget);
	}

	void OpenGLTexture2D::Bind(GLenum _textureUnit)
	{
		glActiveTexture(_textureUnit);
		glBindTexture(m_textureTarget, m_textureObj);
	}

	void OpenGLTexture2D::Unbind()
	{
		glBindTexture(m_textureTarget, 0);
	}

	// OpenGLCubeTexture
	OpenGLCubeTexture::OpenGLCubeTexture(const TextureSpecification& _specifications, std::vector<uint8_t*> _vpData, bool _bCompressed)
		: m_specifications(_specifications),
		m_imageWidth(_specifications.iWidth),
		m_imageHeight(_specifications.iHeight),
		m_textureTarget(GL_TEXTURE_CUBE_MAP)
	{
		if (_bCompressed)
		{
			LoadInternalCompressed(_vpData);
		}
		else
		{
			LoadInternal(_vpData);
		}
	}

	void OpenGLCubeTexture::LoadInternal(const std::vector<uint8_t*> _vpData)
	{
		glGenTextures(1, &m_textureObj);
		glBindTexture(m_textureTarget, m_textureObj);

		glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		GLenum glFormat = ImageFormatToGLFormat(m_specifications.format);

		int iSize = (int)_vpData.size();
		for (int iFace = 0; iFace < iSize; iFace++)
		{
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace,
				0,
				glFormat,
				m_imageWidth,
				m_imageHeight,
				0,
				glFormat,
				GL_UNSIGNED_BYTE,
				_vpData[iFace]
			);
		}

		glBindTexture(m_textureTarget, 0);
	}

	void OpenGLCubeTexture::LoadInternalCompressed(const std::vector<uint8_t*> _vpData)
	{
		glGenTextures(1, &m_textureObj);
		glBindTexture(m_textureTarget, m_textureObj);

		glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		GLenum glFormat = CompressedImageFormatToGLFormat(m_specifications.format);

		int iSize = (int)_vpData.size();
		for (int iFace = 0; iFace < iSize; iFace++)
		{
			glCompressedTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace,
				0,
				glFormat,
				m_specifications.iWidth,
				m_specifications.iHeight,
				0,
				m_specifications.iDataSize,
				_vpData[iFace]
			);
		}

		glBindTexture(m_textureTarget, 0);
	}

	void OpenGLCubeTexture::Bind(GLenum _textureUnit)
	{
		glActiveTexture(_textureUnit);
		glBindTexture(m_textureTarget, m_textureObj);
	}

	void OpenGLCubeTexture::Unbind()
	{
		glBindTexture(m_textureTarget, 0);
	}
}