#pragma once

#include <memory>
#include <string>

#include "../Core/UUID.h"

namespace Omotura
{
	using AssetHandle = UUID;

	enum AssetType : uint16_t
	{
		NONE = 0,
		ANIMATION,
		MODEL,
		SKINNEDMODEL,
		MATERIAL,
		TEXTURE2D,
		CUBETEXTURE
	};

	std::string GetStringFromType(AssetType _type);
	AssetType GetTypeFromString(std::string _strType);

	class Asset
	{
	public:
		AssetHandle m_handle;

		virtual AssetType GetType() const = 0;
	};
}