#pragma once

#include "../../Utils/Math.hpp"

#include "../../Renderer/Texture.h"
#include "../../Renderer/Mesh.h"

#include <vector>

namespace Omotura
{
	struct Region
	{
		float fHeight;
		Vector3 vColor;

		Region(float _fHeight, Vector3 _vColor) : fHeight(_fHeight), vColor(_vColor) {}
	};

	class Terrain
	{
	private:
		GLuint m_VAO;

		std::vector<Region> m_vRegions;
		std::vector<std::vector<float>> m_vfNoiseMap;
		Shared<Texture2D> m_pHeightMap;
		Shared<Texture2D> m_pTexture;

		std::vector<Shared<Texture2D>> m_vpTextures;

		Mesh m_mesh;

	public:
		Terrain();
		~Terrain() = default;

		void GenerateTerrain(int _iMapWidth, int _iMapHeight, int _iSeed, int _iScale, int _iOctaves, float _fPersistance, float _fLacunarity);

		void AddRegion(const Region& _region);
		Shared<Texture2D> GetTexture();
		std::vector<Shared<Texture2D>> GetTextures();
		Shared<Texture2D> GetHeightMap();
		GLuint GetVAO();
		float GetHeightInterpolated(float _fX, float _fZ);

		int GetNumVertices();
	};
}