#pragma once

#include "AudioImporter.h"

namespace Omotura
{
	Shared<AudioClip> AudioImporter::LoadAudioClip(const AssetInfo& _infos)
	{
		return CreateShared<AudioClip>(std::get<std::string>(_infos.strFilePath), _infos.strName);
	}
}