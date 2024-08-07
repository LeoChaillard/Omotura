#include "AssetImporter.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "AnimationImporter.h"
#include "AudioImporter.h"
#include "FontImporter.h"
#include "ShaderImporter.h"

#include <memory>
#include <map>
#include <functional>

namespace Omotura
{
	using AssetImportFunction = std::function<Shared<Asset>(const AssetInfo&)>;

	static std::map<AssetType, AssetImportFunction> s_assetImportFunctions =
	{
		{ AssetType::TEXTURE2D, TextureImporter::LoadCompressedTexture2D },
		{ AssetType::CUBETEXTURE, TextureImporter::LoadCompressedCubeTexture },
		{ AssetType::MESH, MeshImporter::LoadMesh },
		{ AssetType::SKINNEDMESH, MeshImporter::LoadSkinnedMesh },
		{ AssetType::ANIMATION, AnimationImporter::LoadAnimation },
		{ AssetType::AUDIO, AudioImporter::LoadAudioClip },
		{ AssetType::FONT, FontImporter::LoadFont },
		{ AssetType::SHADER, ShaderImporter::LoadShader }
	};

	Shared<Asset> AssetImporter::LoadAsset(const AssetInfo& _infos)
	{
		if (s_assetImportFunctions.find(_infos.type) == s_assetImportFunctions.end())
		{
			std::cout << "No importer available for asset type" << std::endl;
			return nullptr;
		}

		AssetImportFunction importFunction = s_assetImportFunctions.at(_infos.type);
		return importFunction(_infos);
	}
}