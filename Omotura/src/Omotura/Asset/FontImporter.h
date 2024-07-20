#pragma once

#include "../Common/Common.h"

#include "../Renderer/Font.h"

namespace Omotura
{
	class FontImporter
	{
	public:
		static Shared<Font> LoadFont(const AssetInfo& _infos);
	};
}