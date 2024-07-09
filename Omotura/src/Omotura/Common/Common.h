#pragma once

#include "../Asset/AssetInfo.h"
#include "../Asset/Asset.h"

#include <memory>
#include <functional>
#include <string>

namespace Omotura
{
	// Hash
	namespace
	{
		std::size_t hashID(const char* _strName)
		{
			return std::hash<std::string>()(std::string(_strName));
		}
	};
	
	// Smart pointers
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Shared = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Shared<T> CreateShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// Callbacks
	using AssetLoadCallback = std::function<void(const AssetInfo&, Shared<Asset>)>;

	// Preprocessor directives
	#define ARRAY_SIZE_IN_ELEMENTS(a) sizeof(a) / sizeof(a[0])
	#define PATH_SEPARATOR "/"
	//#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals /*| aiProcess_FlipUVs */ | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights | aiProcess_CalcTangentSpace)
	#define ASSIMP_LOAD_FLAGS ( aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_LimitBoneWeights | aiProcess_RemoveRedundantMaterials | aiProcess_SplitLargeMeshes | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType)

}
