#include "SkinnedModelImporter.h"

namespace Omotura
{
	Shared<SkinnedModel> SkinnedModelImporter::LoadSkinnedModel(const AssetInfo& _infos)
	{
		return CreateShared<SkinnedModel>(std::get<std::string>(_infos.strFilePath));
	}
}