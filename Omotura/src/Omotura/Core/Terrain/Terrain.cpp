#include "Terrain.h"
#include "Noise.h"
#include "TextureGenerator.h"

#include "../../Utils/Utils.hpp"

#include "../../Asset/AssetManager.h"

namespace Omotura
{
	namespace constants
	{
		constexpr int iDefaultChunk = 512;
		constexpr int iTextureChannels = 1;
		constexpr int iNumPatchPoints = 4;
		constexpr int iRes = 20;
	}

	Terrain::Terrain()
		: m_vfNoiseMap(),
		m_mesh()
	{		
	}

	void Terrain::Init()
	{
		GenerateTerrain(constants::iDefaultChunk, constants::iDefaultChunk, 21, 100, 12, 0.5f, 2.0f);
	}

	void Terrain::GenerateTerrain(int _iMapWidth, int _iMapHeight, int _iSeed, int _iScale, int _iOctaves, float _fPersistance, float _fLacunarity)
	{
		// Generate noise map
		m_vfNoiseMap = Noise::GeneratePerlinNoiseMap(_iMapWidth, _iMapHeight, _iSeed, _iScale, _iOctaves, _fPersistance, _fLacunarity);

		// Initiating pixel array
		uint8_t* pHeights = new uint8_t[_iMapWidth * _iMapHeight * constants::iTextureChannels];
		for (int x = 0; x < _iMapWidth; x++)
		{
			for (int y = 0; y < _iMapHeight; y++)
			{
				int iIndexHeight = (y * _iMapWidth + x)  * constants::iTextureChannels;
				int iIndexPixel = (y * _iMapWidth + x)  * 4;
		
				// Convert -1 to 1 into 255 to 0	
				float fHeight = (m_vfNoiseMap[x][y] + 1.0f) * 0.5f;
				pHeights[iIndexHeight] = (uint8_t)(fHeight * 255.0f);
			}
		}
		
		// Generate mesh vertices
		std::vector<Vector3> vVertices;
		std::vector<Vector2> vUVs;
		vVertices.reserve(constants::iRes * constants::iRes * 4);
		vUVs.reserve(constants::iRes * constants::iRes * 4);

		int iRes = constants::iRes;
		for (int i = 0; i <= iRes - 1; i++)
		{
			for (int j = 0; j <= iRes - 1; j++)
			{
				vVertices.push_back(Vector3(-_iMapHeight / 2.0f + _iMapHeight * j / (float)iRes, // v.z
									0.0f, // v.y
									-_iMapWidth / 2.0f + _iMapWidth * i / (float)iRes)); // v.x
				vUVs.push_back(Vector2(i / (float)iRes, // u
									  j / (float)iRes)); // v

				vVertices.push_back(Vector3(-_iMapHeight / 2.0f + _iMapHeight * j / (float)iRes, // v.z
											0.0f, // v.y
											-_iMapWidth / 2.0f + _iMapWidth * (i + 1) / (float)iRes)); // v.x
				vUVs.push_back(Vector2((i + 1) / (float)iRes, // u
									   j / (float)iRes)); // v

				vVertices.push_back(Vector3(-_iMapHeight / 2.0f + _iMapHeight * (j + 1) / (float)iRes, // v.z
											0.0f, // v.y
											-_iMapWidth / 2.0f + _iMapWidth * i / (float)iRes)); // v.x
				vUVs.push_back(Vector2(i  / (float)iRes, // u
									 (j + 1) / (float)iRes)); // v

				vVertices.push_back(Vector3(-_iMapHeight / 2.0f + _iMapHeight * (j + 1) / (float)iRes, // v.z
											0.0f, // v.y
											-_iMapWidth / 2.0f + _iMapWidth * (i + 1) / (float)iRes)); // v.x
				vUVs.push_back(Vector2((i + 1) / (float)iRes, // u
									  (j + 1) / (float)iRes)); // v			
			}
		}
		m_mesh.SetUVs(vUVs);
		m_mesh.SetVertices(vVertices);
		m_mesh.SetTopology(MeshTopology::PATCHES);

		// Create a submesh
		std::vector<SubMeshDescriptor> vSubMeshes;
		vSubMeshes.push_back(SubMeshDescriptor(0, (uint32_t)vVertices.size(), 0, 0, 0));
		m_mesh.SetSubMeshes(vSubMeshes);
			
		// Set vertices per patch
		glPatchParameteri(GL_PATCH_VERTICES, constants::iNumPatchPoints);

		// Settings the specs
		TextureSpecification specs;
		specs.iWidth = _iMapWidth;
		specs.iHeight = _iMapHeight;
		specs.format = ImageFormat::RED;
		
		// Create height map texture
		Shared<Texture> pHeightMap = CreateShared<OpenGLTexture2D>(specs, pHeights, false);
		AssetManager::AddAsset(hashID("HeightMap"), pHeightMap);
		
		// Set materials
		std::vector<Material> vMaterials;
		vMaterials.push_back(Material(hashID("HeightMap")));
		vMaterials.push_back(Material(hashID("Water")));
		vMaterials.push_back(Material(hashID("Grass")));
		vMaterials.push_back(Material(hashID("Rocks 1")));
		vMaterials.push_back(Material(hashID("Rocks 2")));
		vMaterials.push_back(Material(hashID("Snow")));
		m_mesh.SetMaterials(vMaterials);

		m_mesh.PopulateBuffers();

		delete[] pHeights;
		pHeights = nullptr;
	}

	float Terrain::GetHeightInterpolated(float _fX, float _fZ)
	{
		float fBaseHeight = (m_vfNoiseMap[(int)_fX][(int)_fZ] + 1.0f) * 0.5f * 64.0f;

		if (((int)_fX + 1 >= 512) || ((int)_fZ + 1 >= 512))
		{
			return fBaseHeight;
		}

		// Interpolating X
		float fNextXHeight = (m_vfNoiseMap[(int)_fX + 1][(int)_fZ] + 1.0f) * 0.5f * 64.0f;
		float fRatioX = _fX - floorf(_fX);
		float fInterpolatedX = (float)(fNextXHeight - fBaseHeight) * fRatioX + (float)fBaseHeight;

		// Interpoalting Z
		float fNextZHeight = (m_vfNoiseMap[(int)_fX][(int)_fZ + 1] + 1.0f) * 0.5f * 64.0f;
		float fRatioZ = _fZ - floorf(_fZ);
		float fInterpolatedZ = (float)(fNextZHeight - fBaseHeight) * fRatioZ + (float)fBaseHeight;

		// Final Height
		float fFinalHeight = (fInterpolatedX + fInterpolatedZ) / 2.0f;

		return fFinalHeight;
	}

	const Mesh& Terrain::GetMesh() const
	{
		return m_mesh;
	}
}