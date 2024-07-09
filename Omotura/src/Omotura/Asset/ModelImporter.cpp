#pragma once

#include "ModelImporter.h"

namespace Omotura
{
	Shared<Model> ModelImporter::LoadModel(const AssetInfo& _infos)
	{
		return CreateShared<Model>(std::get<std::string>(_infos.strFilePath));
	}
}