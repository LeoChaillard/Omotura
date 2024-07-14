#pragma once

#include "../Audio/AudioClip.h"
#include "../Common/Common.h"

namespace Omotura
{
	class AudioImporter
	{
	public:
		static Shared<AudioClip> LoadAudioClip(const AssetInfo& _infos);
	};
}