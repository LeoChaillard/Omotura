#pragma once

#include "../../Utils/Math.hpp"

#include "../../Renderer/Material.h"
#include "../../Renderer/Texture.h"
#include "../../Renderer/Mesh.h"

#include <vector>

namespace Omotura
{
	class Terrain
	{
	private:
		std::vector<std::vector<float>> m_vfNoiseMap;
		Mesh m_mesh;

	public:
		Terrain();
		~Terrain() = default;

		void Init();

		void GenerateTerrain(int _iMapWidth, int _iMapHeight, int _iSeed, int _iScale, int _iOctaves, float _fPersistance, float _fLacunarity);
		float GetHeightInterpolated(float _fX, float _fZ);

		const Mesh& GetMesh() const;
	};
}