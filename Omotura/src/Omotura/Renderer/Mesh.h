#pragma once

#include <memory>

#define INVALID_MATERIAL 0xFFFFFFFF

namespace Omotura
{
	struct Mesh
	{
		uint32_t iNumIndices;
		uint32_t iBaseVertex;
		uint32_t iBaseIndex;
		uint32_t iMaterialIndex;

		Mesh() : iNumIndices(), iBaseVertex(), iBaseIndex(), iMaterialIndex(INVALID_MATERIAL) {}
	};
}
