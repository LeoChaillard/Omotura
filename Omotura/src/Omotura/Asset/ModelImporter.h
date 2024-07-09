#pragma once

#include "AssetInfo.h"

#include "../Common/Common.h"
#include "../Renderer/Model.h"

#include <vector>

namespace Omotura
{
	class ModelImporter
	{
	public:
		static Shared<Model> LoadModel(const AssetInfo& _infos);
	};
}