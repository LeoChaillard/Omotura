#include "Texture.h"

#include "../API/OpenGLTexture.h"

namespace Omotura
{
    Shared<Texture2D> Texture2D::Create(const TextureSpecification& _specifications, uint8_t* _pData, bool _bCompressed /*= false*/)
    {
        return CreateShared<OpenGLTexture2D>(_specifications, _pData, _bCompressed);
    }

    Shared<CubeTexture> CubeTexture::Create(const TextureSpecification& _specifications, std::vector<uint8_t*> _vpData, bool _bCompressed /*= false*/)
    {
        return CreateShared<OpenGLCubeTexture>(_specifications, _vpData, _bCompressed);
    }
}
