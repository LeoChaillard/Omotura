#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../Core/Camera.h"
#include "../Core/Light.h"

#include "Mesh.h"
#include "Model.h"
#include "SkinnedModel.h"

namespace Omotura
{
	struct RenderItem3D
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 inverseModelMatrix = glm::mat4(1.0f);
		Shared<Model> model;
	};

	struct SkinnedRenderItem3D
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 inverseModelMatrix = glm::mat4(1.0f);
		std::vector<glm::mat4> vBoneTransforms;
		Shared<SkinnedModel> skinnedModel;
	};

	struct RenderData
	{
		// Render Items
		std::vector<SkinnedRenderItem3D> vGeometrySkinnedRenderItems;
		std::vector<RenderItem3D> vGeometryRenderItems;
		Shared<Model> pSkyboxModel;

		// Lights
		DirLight dirLight;
		std::vector<PointLight> vPointLights;
		std::vector<SpotLight> vSpotLights;

		// Camera
		Shared<Camera> pPlayerCamera;
	};
}