#include "AssetManager.h"
#include "AssetImporter.h"

#include "../Utils/Utils.hpp"
#include "../Common/Common.h"

namespace Omotura
{
	namespace constants
	{
		constexpr int CUBE_FACES = 6;
	}

	// Types to scan
	std::vector<AssetType> AssetManager::m_vAssetTypesToScan =
	{
		AssetType::TEXTURE2D,
		AssetType::CUBETEXTURE,
		AssetType::SKINNEDMODEL,
		AssetType::MODEL,
		AssetType::ANIMATION,
		AssetType::AUDIO,
		AssetType::FONT
	};

	// Allowed extensions
	std::map<AssetType, std::filesystem::path> AssetManager::m_assetTypeExtensions =
	{
		{ AssetType::TEXTURE2D, ".png .jpg .jpeg" },
		{ AssetType::CUBETEXTURE, ".png" },
		{ AssetType::MODEL, ".fbx .obj" },
		{ AssetType::SKINNEDMODEL, ".gltf .glb" },
		{ AssetType::ANIMATION, ".fbx" },
		{ AssetType::AUDIO, ".wav" },
		{ AssetType::FONT, ".ttf" }
	};

	std::vector<std::string> AssetManager::m_vstrAssetsLogs;

	AssetMap AssetManager::m_assets;
	std::map<AssetType, AssetList> AssetManager::m_assetListMap;

	void AssetManager::Init()
	{
		// Retrieve resources folder
		std::string strResourcesFolder = Utils::GetResourcesFolder();

		// Scan asset directories
		for (AssetType type : m_vAssetTypesToScan)
		{
			std::string strDirPath = strResourcesFolder + PATH_SEPARATOR + Utils::GetAssetFolderNameFromType(type);
			ScanAssetPaths(type, strDirPath);
			LoadAssets(m_assetListMap[type]);
			m_vstrAssetsLogs.clear();
		}
	}

	void AssetManager::ScanAssetPaths(AssetType _type, const std::string& _strAssetDirPath)
	{
		// Scan and create AssetInfo for each asset
		AssetList assetList;
		auto it_end = std::filesystem::end(std::filesystem::directory_iterator(_strAssetDirPath));
		auto it_begin = std::filesystem::begin(std::filesystem::directory_iterator(_strAssetDirPath));
		for (auto& entry = it_begin; entry != it_end; entry++)
		{
			std::filesystem::path strFilePath(entry->path());			
			if (!entry->is_directory() && IsExtensionValid(_type, strFilePath.extension().string()))
			{
				AssetInfo infos;				
				infos.type = _type;

				// Handle mutliple paths for cube textures
				if (infos.type == AssetType::CUBETEXTURE)
				{
					std::vector<std::string> strCubeFaces;
					strCubeFaces.push_back(entry->path().string());
					AddAssetToLog(strFilePath.filename().string());
					for (int i = 0; i < constants::CUBE_FACES - 1; i++)
					{
						entry++;
						strCubeFaces.push_back(entry->path().string());
						strFilePath = entry->path();
						AddAssetToLog(strFilePath.filename().string());
					}
					size_t pos = strFilePath.filename().string().find_first_of('_');
					std::string strName = strFilePath.filename().string().substr(0, pos);
					infos.strName = strName;
					infos.strFilePath = strCubeFaces;
				}
				else
				{				
					infos.strName = strFilePath.filename().replace_extension().string();
					infos.strFilePath = strFilePath.string();
					AddAssetToLog(strFilePath.filename().string());
				}
				assetList.push_back(infos);
			}
		}
		m_assetListMap.insert({ _type, assetList });
	}

	void AssetManager::LoadAssets(const AssetList& _assetList)
	{
		for (int i = 0; i < _assetList.size(); i++)
		{
			const AssetInfo& infos = _assetList[i];
			Shared<Asset> asset = AssetImporter::LoadAsset(infos);
			if (asset != nullptr)
			{
				asset->m_strName = infos.strName;
				m_assets.insert({ hashID(infos.strName.c_str()), asset });
				PrintLog(i, infos.type);
			}
		}
	}

	void AssetManager::AddAssetToLog(std::string _strAsset)
	{
		m_vstrAssetsLogs.push_back(_strAsset);
	}

	const std::vector<std::string>& AssetManager::GetAssetsLogs()
	{
		return m_vstrAssetsLogs;
	}

	void AssetManager::PrintLog(int _iCurrentIndex, AssetType _type)
	{
		if (_type == AssetType::CUBETEXTURE)
		{
			for (int j = 0; j < constants::CUBE_FACES; j++)
			{
				printf("Loaded %s\n", (Utils::GetAssetFolderNameFromType(_type) + PATH_SEPARATOR + m_vstrAssetsLogs[_iCurrentIndex + j]).c_str());
			}
		}
		else
		{
			printf("Loaded %s\n", (Utils::GetAssetFolderNameFromType(_type) + PATH_SEPARATOR + m_vstrAssetsLogs[_iCurrentIndex]).c_str());
		}
	}
	
	bool AssetManager::IsExtensionValid(AssetType _type, std::string _strExtension)
	{
		return m_assetTypeExtensions.find(_type)->second.string().find(_strExtension) != std::string::npos;
	}

}