#include "STBImage.h"

#include <stb/stb_image.h>

namespace Omotura
{
	STBImage::~STBImage()
	{
		//if (m_pImageData != nullptr)
		//{
		//	Unload();
		//}
	}

	void STBImage::Load(const char* _pFileName)
	{
		if (m_pImageData != nullptr)
		{
			Unload();
		}

		stbi_set_flip_vertically_on_load(true);

		m_pImageData = stbi_load(_pFileName, &m_iWidth, &m_iHeight, &m_iBPP, 0);

		if (m_pImageData == nullptr)
		{
			exit(0);
		}
	}

	void STBImage::Unload()
	{
		if (m_pImageData == nullptr)
		{
			exit(0);
		}

		stbi_image_free(m_pImageData);
		m_pImageData = nullptr;
		m_iWidth = 0;
		m_iWidth = 0;
		m_iBPP = 0;
	}

	Vector3 STBImage::GetColor(int _iX, int _iY) const
	{
		if (m_pImageData == nullptr)
		{
			return Vector3(0.0f, 0.0f, 0.0f);
		}

		assert(m_iWidth > 0);
		assert(m_iHeight > 0);

		int iWrappedX = _iX % m_iWidth;
		int iWrappedY = _iY % m_iHeight;

		Vector3 vColor;

		uint8_t* pData = m_pImageData + (iWrappedY * m_iWidth + iWrappedX) * m_iBPP;
		vColor.x = (float)pData[0];
		vColor.y = (float)pData[1];
		vColor.z = (float)pData[2];

		return vColor;
	}
}