#pragma once

#include "Asset.h"
#include "AssetInfo.h"

#include "../Common/Common.h"

namespace Omotura
{
	class AssetImporter
	{
	public:
		static Shared<Asset> LoadAsset(const AssetInfo& _infos);
	};
}