#pragma once

#include "../../API/OpenGLTexture.h"

#include <random>

namespace Omotura
{
	class Noise
	{
	private:
		static std::mt19937 m_mt;
		static std::uniform_real_distribution<float> m_dist;

	public:
		static std::vector<std::vector<float>> GeneratePerlinNoiseMap(int _iMapWidth, int _iMapHeight, int _iSeed, int _iScale, int _iOctaves, float _fPersistance, float _fLacunarity);
	};
}