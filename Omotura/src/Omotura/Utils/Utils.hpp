#pragma once

#include "../Asset/Asset.h"

#include <filesystem>
#include <string>

#include <fstream>
#include <sstream>

namespace Omotura
{
	namespace
	{
		const char* m_pResourcesFolderRelative = "/res";
		const char* m_pCookingFolderRelative = "/res/cooking";
		const char* m_pEngineName = "Omotura";
		const char* m_pNoTextureName = "NoTexture";
	}

	class Utils
	{

	public:
		static const std::string& GetResourcesFolder()
		{
			static std::string strResourcesFolder;
			if (strResourcesFolder.empty())
			{
				std::string strCurrentFolder = std::filesystem::current_path().string();
				strResourcesFolder = strCurrentFolder + m_pResourcesFolderRelative;
			}
			return strResourcesFolder;
		}

		static const std::string& GetCookingFolder()
		{
			static std::string strCookingFolder;
			if (strCookingFolder.empty())
			{
				std::string strCurrentFolder = std::filesystem::current_path().string();
				strCookingFolder = strCurrentFolder + m_pCookingFolderRelative;
			}
			return strCookingFolder;
		}

		static const std::string& GetEngineName()
		{
			static std::string strEngineName;
			if (strEngineName.empty())
			{
				strEngineName = m_pEngineName;
			}
			return strEngineName;
		}

		static const std::string& GetNoTextureName()
		{
			static std::string strNoTexture;
			if (strNoTexture.empty())
			{
				strNoTexture = m_pNoTextureName;
			}
			return strNoTexture;
		}

		static std::string GetAssetFolderNameFromType(AssetType _type)
		{
			switch (_type)
			{
				case AssetType::ANIMATION: return "animations";
				case AssetType::MESH: return "models";
				case AssetType::SKINNEDMESH: return "models";
				case AssetType::TEXTURE2D: return "textures";
				case AssetType::CUBETEXTURE: return "textures/skybox";
				case AssetType::AUDIO: return "audio";
				case AssetType::FONT: return "fonts";
				case AssetType::SHADER: return "shaders";
			}
			return std::string();
		}

		static std::string GetFileContents(const char* _pFileName)
		{
			std::ifstream in(_pFileName, std::ios::binary);
			if (in)
			{
				std::string strContents;
				in.seekg(0, std::ios::end);
				strContents.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&strContents[0], strContents.size());
				in.close();
				return strContents;
			}
			throw(errno);
		}
	};
}