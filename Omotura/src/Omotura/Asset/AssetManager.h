#pragma once

#include "AssetInfo.h"

#include "../Common/Common.h"
#include "../Renderer/Model.h"
#include "../Renderer/Texture.h"

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <filesystem>

namespace Omotura
{
	using AssetList = std::vector<AssetInfo>;
	using AssetMap = std::map<AssetHandle, Shared<Asset>>;

	class AssetManager
	{
	private:
		static std::vector<AssetType> m_vAssetTypesToScan;
		static std::map<AssetType, std::filesystem::path> m_assetTypeExtensions;
		static std::vector<std::string> m_vstrAssetsLogs;

		static AssetMap m_assets;
		static std::vector<Shared<Model>> m_vModels;
		static std::vector<Shared<Texture>> m_vTextures;
		static std::map<AssetType, AssetList> m_assetListMap;

	public:
		// Assets loading
		static void Init();

		// Assets getters
		template<typename T>
		static Shared<T> GetAsset(AssetHandle _handle)
		{
			Shared<Asset> asset = m_assets[_handle];
			return std::static_pointer_cast<T>(asset);
		}

		static const std::vector<Shared<Model>>& GetModels()
		{
			return m_vModels;
		}

		static const std::vector<Shared<Texture>>& GetTextures()
		{
			return m_vTextures;
		}

	private:
		// Assets loading
		static void ScanAssetPaths(AssetType _type, const std::string& _strAssetDirPath);
		static void LoadAssets(const AssetList& _assetList);

		// Logs
		static void AddAssetToLog(std::string _strAsset);
		static const std::vector<std::string>& GetAssetsLogs();
		static void PrintLog(int _iCurrentIndex, AssetType _type);

		static std::string GetAssetFolderNameFromType(AssetType _type);
		static bool IsExtensionValid(AssetType _type, std::string _strExtension);
	};
}
