#include "Asset.h"

namespace Omotura
{
	std::string GetStringFromType(AssetType _type)
	{
		switch (_type)
		{
			case AssetType::NONE:      return "AssetType::None";
			case AssetType::MODEL:     return "AssetType::Model";
			case AssetType::MATERIAL: return "AssetType::Material";
			case AssetType::TEXTURE2D: return "AssetType::Texture2D";
			case AssetType::CUBETEXTURE: return "AssetType::CubeTexture";
		}
		return "AssetType::<Invalid>";
	}

	AssetType GetTypeFromString(std::string _strType)
	{
		if (_strType == "AssetType::None")      return AssetType::NONE;
		if (_strType == "AssetType::Model")     return AssetType::MODEL;
		if (_strType == "AssetType::Material") return AssetType::MATERIAL;
		if (_strType == "AssetType::Texture2D") return AssetType::TEXTURE2D;
		if (_strType == "AssetType::CubeTexture") return AssetType::CUBETEXTURE;
		return AssetType::NONE;
	}
}