#pragma once

#include "Terrain.h"
#include "STBImage.h"

#include "../../Renderer/Texture.h"


namespace Omotura
{
	struct TextureHeightDesc
	{
		float fLow;
		float fOptimal;
		float fHigh;
	};

	struct TextureTile
	{
		STBImage image;
		TextureHeightDesc heightDesc;
	};

	class TextureGenerator
	{
	private:
		std::vector<TextureTile> m_vTiles;

	public:
		TextureGenerator() = default;
		~TextureGenerator() = default;

		void AddTile(const char* _pFilePath);

		Shared<Texture2D> GenerateTexture(int _iTextureSize, Terrain& _terrain, float _fMinHeight, float _fMaxHeight);

	private:
		void CalcTextureRegions(float _fMinHeight, float _fMaxHeight);
		float RegionPercent(int _iTile, float _fHeight);
	};
}