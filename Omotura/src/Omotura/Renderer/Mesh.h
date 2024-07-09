#pragma once

#include <memory>

#define INVALID_MATERIAL 0xFFFFFFFF

namespace Omotura
{
	struct Mesh
	{
		Mesh()
		{
			iNumIndices = 0;
			iBaseVertex = 0;
			iBaseIndex = 0;
			iMaterialIndex = INVALID_MATERIAL;
		}

		uint32_t iNumIndices;
		uint32_t iBaseVertex;
		uint32_t iBaseIndex;
		uint32_t iMaterialIndex;
	};
}
