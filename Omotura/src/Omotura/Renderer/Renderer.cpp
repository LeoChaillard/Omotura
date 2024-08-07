#include "Renderer.h"
#include "RendererCommon.h"

#include "../API/OpenGLRendererAPI.h"
#include "../BackEnd/BackEnd.h"
#include "../Asset/AssetManager.h"
#include "../Asset/AssetImporter.h"
#include "../Asset/AssetInfo.h"
#include "../Asset/Asset.h"
#include "../Utils/Utils.hpp"
#include "../Core/Game.h"
#include "../Core/Time.h"

namespace Omotura
{
	extern Game g_gameInstance;

	Scope<RendererAPI> Renderer::s_pRendererAPI;

	void Renderer::Init()
	{
		// Init API
		if (BackEnd::GetAPI() == API::OPENGL)
		{
			s_pRendererAPI = CreateScope<OpenGLRendererAPI>();
			s_pRendererAPI->Init();
		}

		// Add cube and quad meshes to the AssetManager
		CreateCubeMesh();		
		CreateQuadMesh();		
	}

	void Renderer::RenderFrame()
	{
		// Render Scene
		RenderData renderData = CreateRenderData();
		RenderScene(renderData);
	}

	void Renderer::RenderScene(const RenderData& _renderData)
	{
		s_pRendererAPI->RenderScene(_renderData);
	}

	RenderData Renderer::CreateRenderData()
	{
		RenderData data;

		// Geometry
		data.vGeometryRenderItems = CreateGeometryRenderItems();

		// Skybox
		data.pSkybox = AssetManager::GetAsset<Mesh>(hashID("Cube"));

		// Terrain
		data.pTerrain = CreateShared<Mesh>(g_gameInstance.GetScene().GetTerrain()->GetMesh());

		// Lights
		data.dirLight = CreateDirLight();
		data.vPointLights = CreatePointLights();
		data.vSpotLights = CreateSpotLights();

		// Camera
		data.pPlayerCamera = g_gameInstance.GetPlayerCamera();

		return data;
	}

	std::vector<RenderItem3D> Renderer::CreateGeometryRenderItems()
	{
		std::vector<RenderItem3D> vRenderItems;
		const std::vector<Shared<Mesh>>& vMeshes = g_gameInstance.GetScene().GetMeshes();
		int iSize = (int)vMeshes.size();
		for (int i = 0; i < iSize; i++)
		{
			Shared<Mesh> pMesh = vMeshes[i];

			// Do not add to render items if not visible
			if (!pMesh->IsVisible())
			{
				continue;
			}

			// Create and setup render item
			RenderItem3D renderItem;
			renderItem.pMesh = pMesh;
			vRenderItems.push_back(renderItem);			
		}

		return vRenderItems;
	}

	DirLight Renderer::CreateDirLight()
	{
		DirLight dirLight;
		dirLight.vDirection = glm::vec3(0.2f, 1.0f, 0.0f);
		dirLight.vAmbient = glm::vec3(0.25f, 0.25f, 0.25f);
		dirLight.vDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		dirLight.vSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
		return dirLight;
	}

	std::vector<PointLight> Renderer::CreatePointLights()
	{
		std::vector<PointLight> vPointLights;
		PointLight pointLight;
		pointLight.vPosition = glm::vec3(0.0f, 5.0f, 2.0f);
		pointLight.fConstant = 1.0f;
		pointLight.fLinear = 0.022f;
		pointLight.fQuadratic = 0.0019f;
		pointLight.vAmbient = glm::vec3(0.25f, 0.25f, 0.25f);
		pointLight.vDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		pointLight.vSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
		vPointLights.push_back(pointLight);
		return vPointLights;
	}

	std::vector<SpotLight> Renderer::CreateSpotLights()
	{
		std::vector<SpotLight> vSpotLights;
		SpotLight spotLight;
		spotLight.vPosition = glm::vec3(2.5f * sin(Time::GetCurrentTime()), 5.0f, 2.5f * cos(Time::GetCurrentTime()));
		spotLight.vDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		spotLight.fInnerCone = 0.95f;
		spotLight.fOuterCone = 0.90f;
		spotLight.vAmbient = glm::vec3(0.25f, 0.25f, 0.25f);
		spotLight.vDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLight.vSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
		vSpotLights.push_back(spotLight);
		return vSpotLights;
	}

	void Renderer::CreateCubeMesh()
	{
		Shared<Mesh> pCubeMesh = CreateShared<Mesh>();

		// Mesh Attributes
		std::vector<Vector3> vVertices
		{
			{ Vector3(-1.0f, -1.0f,  1.0f) }, //        7--------6
			{ Vector3( 1.0f, -1.0f,  1.0f) }, //       /|       /|
			{ Vector3( 1.0f, -1.0f, -1.0f) }, //      4--------5 |
			{ Vector3(-1.0f, -1.0f, -1.0f) }, //      | |      | |
			{ Vector3(-1.0f,  1.0f,  1.0f) }, //      | 3------|-2
			{ Vector3( 1.0f,  1.0f,  1.0f) }, //      |/       |/
			{ Vector3( 1.0f,  1.0f, -1.0f) }, //      0--------1
			{ Vector3(-1.0f,  1.0f, -1.0f) }
		};

		std::vector<int> vIndices
		{
			// Right
			1, 2, 6, 6, 5, 1,
			// Left
			0, 4, 7, 7, 3, 0,
			// Top
			4, 5, 6, 6, 7, 4,
			// Bottom
			0, 3, 2, 2, 1, 0,
			// Back
			0, 1, 5, 5, 4, 0,
			// Front
			3, 7, 6, 6, 2, 3
		};

		// Create a sub mesh and setting attributes
		std::vector<SubMeshDescriptor> vSubMeshes;
		vSubMeshes.push_back(SubMeshDescriptor((uint32_t)vIndices.size(), (uint32_t)vVertices.size(), 0, 0, 0));
		pCubeMesh->SetSubMeshes(vSubMeshes);
		pCubeMesh->SetVertices(vVertices);
		pCubeMesh->SetIndices(vIndices);

		// Upload data to gpu
		pCubeMesh->PopulateBuffers();

		// Add Mesh to the AssetManager
		AssetManager::AddAsset(hashID("Cube"), pCubeMesh);
	}

	void Renderer::CreateQuadMesh()
	{
		Shared<Mesh> pQuadMesh = CreateShared<Mesh>();

		// Mesh Attributes
		std::vector<Vector3> vVertices
		{
			{ Vector3(-1.0f,  1.0f, 0.0f) },
			{ Vector3(1.0f,  1.0f, 0.0f)  },
			{ Vector3(-1.0f, -1.0f, 0.0f) },
			{ Vector3(1.0f,  -1.0f, 0.0f) },
			{ Vector3(-1.0f, -1.0f, 0.0f) },
			{ Vector3(1.0f,   1.0f, 0.0f) }
		};

		std::vector<int> vIndices
		{
			0, 1, 2, 3, 4, 5
		};

		std::vector<Vector2> vUVs
		{
			Vector2(0.0f, 1.0f),
			Vector2(1.0f, 1.0f),
			Vector2(0.0f, 0.0f),
			Vector2(1.0f, 0.0f),
			Vector2(0.0f, 0.0f),
			Vector2(1.0f, 1.0f),
		};

		// Create a sub mesh and setting attributes
		std::vector<SubMeshDescriptor> vSubMeshes;
		vSubMeshes.push_back(SubMeshDescriptor((uint32_t)vIndices.size(), (uint32_t)vVertices.size(), 0, 0, 0));
		pQuadMesh->SetSubMeshes(vSubMeshes);
		pQuadMesh->SetVertices(vVertices);
		pQuadMesh->SetIndices(vIndices);
		pQuadMesh->SetUVs(vUVs);

		// Upload data to gpu
		pQuadMesh->PopulateBuffers();

		// Add Mesh to the AssetManager
		AssetManager::AddAsset(hashID("Quad"), pQuadMesh);
	}

	void Renderer::Clear()
	{
		s_pRendererAPI->Clear();		
	}
}