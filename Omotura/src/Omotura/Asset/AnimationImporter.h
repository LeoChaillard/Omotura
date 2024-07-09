#pragma once

#include "AssetInfo.h"

#include "../Common/Common.h"
#include "../Core/Animation.h"

namespace Omotura
{
	class AnimationImporter
	{
	public:
		static Shared<Animation> LoadAnimation(const AssetInfo& _infos);
	};
}