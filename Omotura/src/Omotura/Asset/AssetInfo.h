#pragma once

#include "Asset.h"

#include <string>
#include <vector>
#include <variant>

namespace Omotura
{
	struct AssetInfo
	{
		std::string strName;
		AssetType type = AssetType::NONE;
		std::variant<std::string, std::vector<std::string>> strFilePath;
	};
}