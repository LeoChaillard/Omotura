#pragma once

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
	};
}