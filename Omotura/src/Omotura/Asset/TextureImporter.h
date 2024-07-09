#pragma once

#include "AssetInfo.h"

#include "../Common/Common.h"
#include "../Renderer/Texture.h"

#include <compressonator/compressonator.h>
#include <compressonator/DDS_Helpers.h>

namespace Omotura
{
	class TextureImporter
	{
	public:
		static Shared<Texture2D> LoadTexture2D(const AssetInfo& _infos);
		static Shared<Texture2D> LoadCompressedTexture2D(const AssetInfo& _infos);
		static Shared<CubeTexture> LoadCubeTexture(const AssetInfo& _infos);
		static Shared<CubeTexture> LoadCompressedCubeTexture(const AssetInfo& _infos);

	private:
		static ImageFormat GetImageFormatFromChannels(int _iChannels);
		static ImageFormat GetCompressedImageFormat(CMP_FORMAT _format);
	};
}