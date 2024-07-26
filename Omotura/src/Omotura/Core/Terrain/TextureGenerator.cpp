#include "TextureGenerator.h"

#include "../../Asset/AssetManager.h"

#include "../../API/OpenGLTexture.h"

namespace Omotura
{
	void TextureGenerator::AddTile(const char* _pFilePath)
	{
		TextureTile tile;
		tile.image.Load(_pFilePath);
		m_vTiles.push_back(tile);
	}

	Shared<Texture2D> TextureGenerator::GenerateTexture(int _iTextureSize, Terrain& _terrain, float _fMinHeight, float _fMaxHeight)
	{
		CalcTextureRegions(_fMinHeight, _fMaxHeight);

		uint8_t* pData = new uint8_t[_iTextureSize * _iTextureSize * 3];

		float HeightMapToTextureRatio = (float)512 / (float)_iTextureSize;

		printf("Height map to texture ratio: %f\n", HeightMapToTextureRatio);

		for (int y = 0; y < _iTextureSize; y++) {
			for (int x = 0; x < _iTextureSize; x++) {

				float InterpolatedHeight = _terrain.GetHeightInterpolated((float)x * HeightMapToTextureRatio,
					(float)y * HeightMapToTextureRatio);

				float fRed = 0.0f;
				float fGreen = 0.0f;
				float fBlue = 0.0f;

				for (int Tile = 0; Tile < m_vTiles.size(); Tile++) {
					Vector3 vColor = m_vTiles[Tile].image.GetColor(x, y);

					float BlendFactor = RegionPercent(Tile, InterpolatedHeight);

					fRed += BlendFactor * vColor.x;
					fGreen += BlendFactor * vColor.y;
					fBlue += BlendFactor * vColor.z;
				}

				if (fRed > 255.0f || fGreen > 255.0f || fBlue > 255.0f) {
					printf("%d:%d: %f %f %f\n", y, x, fRed, fGreen, fBlue);
					exit(0);
				}
				
				int iIndex = (y * _iTextureSize + x) * 3;
				pData[iIndex] = (unsigned char)fRed;
				pData[iIndex + 1] = (unsigned char)fGreen;
				pData[iIndex + 2] = (unsigned char)fBlue;
			}
		}

		// Settings the specs
		TextureSpecification specs;
		specs.iWidth = _iTextureSize;
		specs.iHeight = _iTextureSize;
		specs.format = ImageFormat::RGB;

		// Creating the texture
		Shared<Texture2D> pTexture = CreateShared<OpenGLTexture2D>(specs, pData, false);

		delete[] pData;
		pData = nullptr;

		return pTexture;
	}

	void TextureGenerator::CalcTextureRegions(float _fMinHeight, float _fMaxHeight)
	{
		int iSize = m_vTiles.size();;
		float fHeightRange = _fMaxHeight - _fMinHeight;
		float fRangePerTile = fHeightRange / iSize;

		float fLastHeight = -1.0f;
		for (int i = 0; i < iSize; i++)
		{
			m_vTiles[i].heightDesc.fLow = fLastHeight + 1.0f;
			fLastHeight += fRangePerTile;
			m_vTiles[i].heightDesc.fOptimal = fLastHeight;
			m_vTiles[i].heightDesc.fHigh = m_vTiles[i].heightDesc.fOptimal + fRangePerTile;
		}
	}

	float TextureGenerator::RegionPercent(int _iTile, float _fHeight)
	{
		float fPercent = 0.0f;

		if (_fHeight < m_vTiles[_iTile].heightDesc.fLow)
		{
			fPercent = 0.0f;
		}
		else if (_fHeight > m_vTiles[_iTile].heightDesc.fHigh)
		{
			fPercent = 0.0f;
		}
		else if (_fHeight < m_vTiles[_iTile].heightDesc.fOptimal)
		{
			float Nom = (float)_fHeight - (float)m_vTiles[_iTile].heightDesc.fLow;
			float Denom = (float)m_vTiles[_iTile].heightDesc.fOptimal - (float)m_vTiles[_iTile].heightDesc.fLow;
			fPercent = Nom / Denom;
		}
		else if (_fHeight >= m_vTiles[_iTile].heightDesc.fOptimal)
		{
			float Nom = (float)m_vTiles[_iTile].heightDesc.fHigh - (float)_fHeight;
			float Denom = (float)m_vTiles[_iTile].heightDesc.fHigh - (float)m_vTiles[_iTile].heightDesc.fOptimal;
			fPercent = Nom / Denom;
		}

		return fPercent;
	}
}