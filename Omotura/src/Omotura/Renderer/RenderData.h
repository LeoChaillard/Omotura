#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../Core/Camera.h"
#include "../Core/Light.h"

#include "Mesh.h"
#include "MeshRenderer.h"

namespace Omotura
{
	struct RenderItem3D
	{
		Shared<Mesh> pMesh;
	};

	struct RenderData
	{
		// Geometry
		std::vector<RenderItem3D> vGeometryRenderItems;

		// Skybox
		Shared<Mesh> pSkybox;

		// Terrain
		Shared<Mesh> pTerrain;

		// Lights
		DirLight dirLight;
		std::vector<PointLight> vPointLights;
		std::vector<SpotLight> vSpotLights;

		// Camera
		Shared<Camera> pPlayerCamera;
	};
}