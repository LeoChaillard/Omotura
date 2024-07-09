#pragma once

#include "../Renderer/Texture.h"

#include <compressonator/compressonator.h>

namespace Omotura
{
	class OpenGLTexture2D : public Texture2D
	{
    public:
        OpenGLTexture2D(const TextureSpecification& _specifications, uint8_t* _pData, bool _bCompressed);
        virtual ~OpenGLTexture2D() = default;

        virtual const TextureSpecification& GetSpecifications() override { return m_specifications; }

        virtual uint32_t GetWidth() const override { return m_imageWidth; }
        virtual uint32_t GetHeight() const override { return m_imageHeight; }
        virtual GLenum GetTextureTarget() const override { return m_textureTarget; }
        virtual GLuint GetTextureRef() const override { return m_textureObj; }

        virtual void Bind(GLenum _textureUnit) override;
        virtual void Unbind() override;

    private:
        void LoadInternal(const void* _pImageData);
        void LoadInternalCompressed(const void* _pImageData);

    private:
        TextureSpecification m_specifications;

        GLenum m_textureTarget;
        GLuint m_textureObj;
        uint32_t m_imageWidth;
        uint32_t m_imageHeight;
	};

    class OpenGLCubeTexture : public CubeTexture
    {
    public:
        OpenGLCubeTexture(const TextureSpecification& _specifications, std::vector<uint8_t*> _vpData, bool _bCompressed);
        virtual ~OpenGLCubeTexture() = default;

        virtual const TextureSpecification& GetSpecifications() override { return m_specifications; }

        virtual uint32_t GetWidth() const override { return m_imageWidth; }
        virtual uint32_t GetHeight() const override { return m_imageHeight; }
        virtual GLenum GetTextureTarget() const override { return m_textureTarget; }
        virtual GLuint GetTextureRef() const override { return m_textureObj; }

        virtual void Bind(GLenum _textureUnit) override;
        virtual void Unbind() override;

    private:
        void LoadInternal(const std::vector<uint8_t*> _vpData);
        void LoadInternalCompressed(const std::vector<uint8_t*> _vpData);

    private:
        TextureSpecification m_specifications;

        GLenum m_textureTarget;
        GLuint m_textureObj;
        uint32_t m_imageWidth;
        uint32_t m_imageHeight;
    };
}