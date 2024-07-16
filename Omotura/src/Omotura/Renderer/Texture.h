#pragma once

#include "Shader.h"

#include "../Common/Common.h"
#include "../Asset/Asset.h"

#include <string>
#include<glad/glad.h>

namespace Omotura
{
    enum class ImageFormat
    {
        NONE = 0,
        RED,
        RG,
        RGB,
        RGBA,
        COMPRESSED_RGB_DXT1,
        COMPRESSED_RGBA_DXT3,
        COMPRESSED_RGBA_DXT5
    };

    struct TextureSpecification
    {
        uint32_t iWidth;
        uint32_t iHeight;
        uint32_t iDataSize;
        ImageFormat format;
        bool bGenerateMips = true;
    };

    class Texture : public Asset
    {
    public:
        virtual ~Texture() = default;

        virtual const TextureSpecification& GetSpecifications() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual GLenum GetTextureTarget() const = 0;
        virtual GLuint GetTextureRef() const = 0;

        virtual void Bind(GLenum _textureUnit) = 0;
        virtual void Unbind() = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Shared<Texture2D> Create(const TextureSpecification& _specifications, uint8_t* _pDatabool, bool _bCompressed = false);

        static AssetType GetStaticType() { return AssetType::TEXTURE2D; }
        virtual AssetType GetType() const { return GetStaticType(); }
    };

    class CubeTexture : public Texture
    {
    public:
        static Shared<CubeTexture> Create(const TextureSpecification& _specifications, std::vector<uint8_t*> _pData, bool _bCompressed = false);

        static AssetType GetStaticType() { return AssetType::CUBETEXTURE; }
        virtual AssetType GetType() const { return GetStaticType(); }
    };
}

