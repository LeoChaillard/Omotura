#include "Renderer.h"
#include "RendererCommon.h"
#include "OpenGLRenderer.h"
#include "SkinnedModel.h"

#include "../BackEnd/BackEnd.h"
#include "../Asset/AssetManager.h"
#include "../Asset/AssetImporter.h"
#include "../Asset/AssetInfo.h"
#include "../Asset/Asset.h"
#include "../Utils/Utils.hpp"
#include "../Core/Game.h"

namespace Omotura
{
	void Renderer::RenderFrame()
	{
		// Global Illumination


		// Render Game
		RenderData renderData = CreateRenderData();
		RenderGame(renderData);
	}

	void Renderer::RenderGame(RenderData _renderData)
	{
		if (BackEnd::GetAPI() == API::OPENGL)
		{
			OpenGLRenderer::GetGLRendererInstance()->RenderGame(_renderData);
		}	
	}

	RenderData Renderer::CreateRenderData()
	{
		RenderData data;

		// Render items
		data.vGeometrySkinnedRenderItems = CreateGeometrySkinnedRenderItems();
		data.vGeometryRenderItems = CreateGeometryRenderItems();
		data.pSkyboxModel = AssetManager::GetAsset<Model>(hashID("Cube"));

		// Lights
		data.dirLight = CreateDirLight();
		data.vPointLights = CreatePointLights();
		data.vSpotLights = CreateSpotLights();

		// Camera
		data.pPlayerCamera = Game::GetInstance()->GetPlayerCamera();

		return data;
	}

	std::vector<SkinnedRenderItem3D> Renderer::CreateGeometrySkinnedRenderItems()
	{
		std::vector<SkinnedRenderItem3D> vSkinnedRenderItems;
		const std::vector<Shared<SkinnedModel>>& vpSkinnedModels = Game::GetInstance()->GetScene().GetSkinnedModels();
		int iSize = vpSkinnedModels.size();
		for (int i = 0; i < iSize; i++)
		{
			Shared<SkinnedModel> pSkinnedModel = vpSkinnedModels[i];
			SkinnedRenderItem3D renderItem;
			renderItem.modelMatrix = pSkinnedModel->GetModelMatrix();
			renderItem.inverseModelMatrix = glm::inverse(pSkinnedModel->GetModelMatrix());
			renderItem.skinnedModel = pSkinnedModel;
			renderItem.vBoneTransforms = Game::GetInstance()->GetScene().GetAllBoneTransforms()[i];
			vSkinnedRenderItems.push_back(renderItem);
		}

		// TEMPORARY LIGHT RENDER ITEM
		//RenderItem3D renderItem;
		//renderItem.modelMatrix = m_pLight->GetModelMatrix();
		//renderItem.inverseModelMatrix = glm::inverse(m_pLight->GetModelMatrix());
		//renderItem.model = m_pLight;
		//vRenderItems.push_back(renderItem);

		return vSkinnedRenderItems;
	}

	std::vector<RenderItem3D> Renderer::CreateGeometryRenderItems()
	{
		std::vector<RenderItem3D> vRenderItems;
		const std::vector<Shared<Model>>& vModels = Game::GetInstance()->GetScene().GetModels();
		for (const Shared<Model>& pModel : vModels)
		{
			RenderItem3D renderItem;
			renderItem.modelMatrix = pModel->GetModelMatrix();
			renderItem.inverseModelMatrix = glm::inverse(pModel->GetModelMatrix());
			renderItem.model = pModel;
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
		spotLight.vPosition = glm::vec3(2.5f * sin(glfwGetTime()), 5.0f, 2.5f * cos(glfwGetTime()));
		spotLight.vDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		spotLight.fInnerCone = 0.95f;
		spotLight.fOuterCone = 0.90f;
		spotLight.vAmbient = glm::vec3(0.25f, 0.25f, 0.25f);
		spotLight.vDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLight.vSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
		vSpotLights.push_back(spotLight);
		return vSpotLights;
	}

	void Renderer::Clear()
	{
		if (BackEnd::GetAPI() == API::OPENGL)
		{
			OpenGLRenderer::GetGLRendererInstance()->Clear();
		}
	}
}