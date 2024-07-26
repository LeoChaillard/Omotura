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
		: m_vRegions(),
		m_pTexture(nullptr),
		m_vfNoiseMap()
	{		
		//m_vRegions.push_back(Region(0.4f, Vector3(0.0f, 0.0f, 1.0f)));
		//m_vRegions.push_back(Region(0.45f, Vector3(0.98f, 0.98f, 0.73f)));
		//m_vRegions.push_back(Region(0.6f, Vector3(0.0f, 1.0f, 0.0f)));
		//m_vRegions.push_back(Region(0.7f, Vector3(0.36f, 0.28f, 0.071f)));
		//m_vRegions.push_back(Region(0.9f, Vector3(0.24f, 0.16f, 0.035f)));
		//m_vRegions.push_back(Region(1.0f, Vector3(0.95f, 0.95f, 0.95f)));
		GenerateTerrain(constants::iDefaultChunk, constants::iDefaultChunk, 21, 100, 12, 0.5f, 2.0f);
	}

	void Terrain::GenerateTerrain(int _iMapWidth, int _iMapHeight, int _iSeed, int _iScale, int _iOctaves, float _fPersistance, float _fLacunarity)
	{
		// Generate noise map
		m_vfNoiseMap = Noise::GeneratePerlinNoiseMap(_iMapWidth, _iMapHeight, _iSeed, _iScale, _iOctaves, _fPersistance, _fLacunarity);

		// Initiating pixel array
		uint8_t* pHeights = new uint8_t[_iMapWidth * _iMapHeight * constants::iTextureChannels];
		//uint8_t* pPixels = new uint8_t[_iMapWidth * _iMapHeight * 4];
		for (int x = 0; x < _iMapWidth; x++)
		{
			for (int y = 0; y < _iMapHeight; y++)
			{
				int iIndexHeight = (y * _iMapWidth + x)  * constants::iTextureChannels;
				int iIndexPixel = (y * _iMapWidth + x)  * 4.0f;
		
				// Convert -1 to 1 into 255 to 0	
				float fHeight = (m_vfNoiseMap[x][y] + 1.0f) * 0.5f;
				pHeights[iIndexHeight] = fHeight * 255.0f;
		
				//for (const Region& region : m_vRegions)
				//{
				//	if (fHeight <= region.fHeight)
				//	{
				//		Vector3 vColor = region.vColor;
				//		pPixels[iIndexPixel] = vColor.x * 255.0f;
				//		pPixels[iIndexPixel + 1] = vColor.y * 255.0f;
				//		pPixels[iIndexPixel + 2] = vColor.z * 255.0f;
				//		pPixels[iIndexPixel + 3] = 255.0f;
				//		break;
				//	}
				//
				//}
			}
		}
		
		// Generate mesh vertices
		std::vector<float> vVertices;
		vVertices.reserve(constants::iRes * constants::iRes * 4 * 5);

		int iRes = constants::iRes;
		for (int i = 0; i <= iRes - 1; i++)
		{
			for (unsigned j = 0; j <= iRes - 1; j++)
			{
				vVertices.push_back(-_iMapWidth / 2.0f + _iMapWidth * i / (float)iRes); // v.x
				vVertices.push_back(0.0f); // v.y
				vVertices.push_back(-_iMapHeight / 2.0f + _iMapHeight * j / (float)iRes); // v.z
				vVertices.push_back(i / (float)iRes); // u
				vVertices.push_back(j / (float)iRes); // v

				vVertices.push_back(-_iMapWidth / 2.0f + _iMapWidth * (i + 1) / (float)iRes); // v.x
				vVertices.push_back(0.0f); // v.y
				vVertices.push_back(-_iMapHeight / 2.0f + _iMapHeight * j / (float)iRes); // v.z
				vVertices.push_back((i + 1) / (float)iRes); // u
				vVertices.push_back(j / (float)iRes); // v

				vVertices.push_back(-_iMapWidth / 2.0f + _iMapWidth * i / (float)iRes); // v.x
				vVertices.push_back(0.0f); // v.y
				vVertices.push_back(-_iMapHeight / 2.0f + _iMapHeight * (j + 1) / (float)iRes); // v.z
				vVertices.push_back(i / (float)iRes); // u
				vVertices.push_back((j + 1) / (float)iRes); // v

				vVertices.push_back(-_iMapWidth / 2.0f + _iMapWidth * (i + 1) / (float)iRes); // v.x
				vVertices.push_back(0.0f); // v.y
				vVertices.push_back(-_iMapHeight / 2.0f + _iMapHeight * (j + 1) / (float)iRes); // v.z
				vVertices.push_back((i + 1) / (float)iRes); // u
				vVertices.push_back((j + 1) / (float)iRes); // v				
			}
		}
			
		// Bind data / attribute
		GLuint terrainVBO;
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		
		glGenBuffers(1, &terrainVBO);
		glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices[0]) * vVertices.size(), vVertices.data(), GL_STATIC_DRAW);
		
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		glPatchParameteri(GL_PATCH_VERTICES, constants::iNumPatchPoints);

		// Settings the specs
		TextureSpecification specs;
		specs.iWidth = _iMapWidth;
		specs.iHeight = _iMapHeight;
		specs.format = ImageFormat::RED;
		
		// Creating the height map
		m_pHeightMap = CreateShared<OpenGLTexture2D>(specs, pHeights, false);

		// Creating the terarin texture
		//specs.format = ImageFormat::RGBA;
		//m_pTexture = CreateShared<OpenGLTexture2D>(specs, pPixels, false);
		
		//TextureGenerator texGen;
		//const std::string& strTextureFolder = Utils::GetResourcesFolder() + PATH_SEPARATOR + Utils::GetAssetFolderNameFromType(AssetType::TEXTURE2D) + PATH_SEPARATOR;
		//texGen.AddTile((strTextureFolder + "Water.png").c_str());
		//texGen.AddTile((strTextureFolder + "Sandy grass.png").c_str());
		//texGen.AddTile((strTextureFolder + "Grass.png").c_str());
		//texGen.AddTile((strTextureFolder + "Rocks 1.png").c_str());
		//texGen.AddTile((strTextureFolder + "Rocks 2.png").c_str());
		//texGen.AddTile((strTextureFolder + "Snow.png").c_str());		
		//m_pTexture = texGen.GenerateTexture(512, *this, 0.0f, 64.0f);

		const std::string& strTextureFolder = Utils::GetResourcesFolder() + PATH_SEPARATOR + Utils::GetAssetFolderNameFromType(AssetType::TEXTURE2D) + PATH_SEPARATOR;
	
		m_vpTextures.push_back(AssetManager::GetAsset<Texture2D>(hashID("Water")));
		m_vpTextures.push_back(AssetManager::GetAsset<Texture2D>(hashID("Grass")));
		m_vpTextures.push_back(AssetManager::GetAsset<Texture2D>(hashID("Rocks 1")));
		m_vpTextures.push_back(AssetManager::GetAsset<Texture2D>(hashID("Rocks 2")));
		m_vpTextures.push_back(AssetManager::GetAsset<Texture2D>(hashID("Snow")));

		delete[] pHeights;
		pHeights = nullptr;

		//delete[] pPixels;
		//pPixels = nullptr;
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


	void Terrain::AddRegion(const Region& _region)
	{
		m_vRegions.push_back(_region);
	}

	Shared<Texture2D> Terrain::GetTexture()
	{
		return m_pTexture;
	}

	std::vector<Shared<Texture2D>> Terrain::GetTextures()
	{
		return m_vpTextures;
	}

	Shared<Texture2D> Terrain::GetHeightMap()
	{
		return m_pHeightMap;
	}

	int Terrain::GetNumVertices()
	{
		return constants::iNumPatchPoints * constants::iRes * constants::iRes;
	}

	GLuint Terrain::GetVAO()
	{
		return m_VAO;
	}
}