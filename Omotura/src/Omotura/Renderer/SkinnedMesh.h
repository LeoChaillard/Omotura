#pragma once

#include "../Utils/Math.hpp"
#include "../Core/GameObject.h"

#include <vector>

namespace Omotura
{
	struct SkinnedMesh : public GameObject
	{
		std::vector<Vector3> vVertices;
		std::vector<Vector3> vNormals;
		std::vector<Vector2> vUV;
		std::vector<int> vIndices;

		std::vector<glm::vec4> vWeights;
		std::vector<glm::ivec4> vBoneIDs;
		std::vector<glm::mat4> vMatrixPalette;

		SkinnedMesh() : vVertices(), vNormals(), vUV(), vIndices(), vWeights(), vBoneIDs(), vMatrixPalette() {}
	};
}
