#pragma once

#include "../../Utils/Math.hpp"

namespace Omotura
{
    class STBImage
    {
    private:
        int m_iWidth;
        int m_iHeight;
        int m_iBPP;
        unsigned char* m_pImageData = nullptr;

    public:
        STBImage() = default;

        ~STBImage();

        void Load(const char* _pFileName);
        void Unload();

        Vector3 GetColor(int _iX, int _iY) const;
    };
}
