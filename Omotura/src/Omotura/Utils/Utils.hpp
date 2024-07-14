#pragma once

#include "../Asset/Asset.h"

#include <filesystem>
#include <string>

namespace Omotura
{
	class Utils
	{
	public:
		static const std::string& GetResourcesFolder()
		{
			static std::string strResourcesFolder;
			if (strResourcesFolder.empty())
			{
				std::string strCurrentFolder = std::filesystem::current_path().string();
				strResourcesFolder = strCurrentFolder + "/res";
			}
			return strResourcesFolder;
		}

		static const std::string& GetCookingFolder()
		{
			static std::string strCookingFolder;
			if (strCookingFolder.empty())
			{
				std::string strCurrentFolder = std::filesystem::current_path().string();
				strCookingFolder = strCurrentFolder + "/res/cooking";
			}
			return strCookingFolder;
		}

		static const std::string& GetEngineName()
		{
			static std::string strEngineName;
			if (strEngineName.empty())
			{
				strEngineName = "Omotura";
			}
			return strEngineName;
		}

		static std::string GetAssetFolderNameFromType(AssetType _type)
		{
			switch (_type)
			{
				case AssetType::ANIMATION: return "animations";
				case AssetType::MODEL: return "models";
				case AssetType::SKINNEDMODEL: return "models";
				case AssetType::TEXTURE2D: return "textures";
				case AssetType::CUBETEXTURE: return "textures/skybox";
				case AssetType::AUDIO: return "audio";
			}
			return std::string();
		}
	};
}