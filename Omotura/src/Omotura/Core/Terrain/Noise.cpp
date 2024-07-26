#include "Noise.h"

#include "../../Utils/Math.hpp"

namespace Omotura
{
	namespace constants
	{
		constexpr int iTextureChannels = 4;
	}

	std::mt19937 Noise::m_mt;
	std::uniform_real_distribution<float> Noise::m_dist;

	std::vector<std::vector<float>> Noise::GeneratePerlinNoiseMap(int _iMapWidth, int _iMapHeight, int _iSeed, int _iScale, int _iOctaves, float _fPersistance, float _fLacunarity)
	{
		std::vector<std::vector<float>> vfNoiseMap(_iMapWidth, std::vector<float>(_iMapHeight));

		// Generating random offsets for each octave
		m_mt = std::mt19937(_iSeed);
		m_dist = std::uniform_real_distribution<float>(0, 100000.0f);
		std::vector<Vector2> vOctaveOffsets(_iOctaves);
		for (int i = 0; i < _iOctaves; i++)
		{
			float fOffSetX = m_dist(m_mt);
			float fOffSetY = m_dist(m_mt);
			vOctaveOffsets[i] = Vector2(fOffSetX, fOffSetY);
		}

		// Filling the map with noise values
		for (int x = 0; x < _iMapWidth; x++)
		{
			for (int y = 0; y < _iMapHeight; y++)
			{
				int iIndex = (y * _iMapWidth + x) * constants::iTextureChannels;

				float fHeight = 0;
				float fFreq = 1;
				float fAmp = 1;

				for (int i = 0; i < _iOctaves; i++)
				{
					fHeight += PerlinNoise((x - _iMapWidth / 2.0f) * fFreq / _iScale + vOctaveOffsets[i].x, (y - _iMapHeight / 2.0f) * fFreq / _iScale + vOctaveOffsets[i].y) * fAmp;
					fFreq *= _fLacunarity;
					fAmp *= _fPersistance;
				}

				// Contrast
				fHeight *= 1.2f;

				// Clipping
				if (fHeight > 1.0f)
				{
					fHeight = 1.0f;
				}
				else if (fHeight < -1.0f)
				{
					fHeight = -1.0f;
				}

				// Set value between 0 and 1
				vfNoiseMap[x][y] = fHeight;
			}
		}
		return vfNoiseMap;
	}
}