#pragma once

#include "AssetInfo.h"

#include "../Common/Common.h"
#include "../Renderer/SkinnedModel.h"

namespace Omotura
{
	class SkinnedModelImporter
	{
	public:
		static Shared<SkinnedModel> LoadSkinnedModel(const AssetInfo& _infos);
	};
}