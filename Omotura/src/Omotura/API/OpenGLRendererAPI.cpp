#include "OpenGLRendererAPI.h"

#include "../Renderer/RendererCommon.h"
#include "../Renderer/Texture.h"
#include "../Renderer/TextureRenderer.h"
#include "../Asset/AssetManager.h"
#include "../Utils/Utils.hpp"
#include "../Input/Input.h"
#include "../Input/Input.h"
#include "../Input/KeyCodes.h"

#include "../Core/DebugHelpers/DebugMenu.h"
#include "../Core/DebugHelpers/DebugDraw.h"
#include "../Core/Terrain/Noise.h"

#include "../Utils/Math.hpp"

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Omotura
{
	namespace constants
	{
		const char* pSyboxTextureName = "NightSky";
	}

	void OpenGLRendererAPI::Init()
	{
		// Shaders
		m_shaders.m_pDefaultShader = AssetManager::GetAsset<Shader>(hashID("default"));
		m_shaders.m_pSkinningShader = AssetManager::GetAsset<Shader>(hashID("skinning"));
		m_shaders.m_pLightShader = AssetManager::GetAsset<Shader>(hashID("light"));
		m_shaders.m_pSkyboxShader = AssetManager::GetAsset<Shader>(hashID("skybox"));
		m_shaders.m_pDebugShader = AssetManager::GetAsset<Shader>(hashID("debug"));
		m_shaders.m_pTesselationShader = AssetManager::GetAsset<Shader>(hashID("tesselation"));

		// Light shader
		glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 2.0f);
		glm::vec3 lightColor = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.25f, 0.25f, 0.25f));
		
		m_shaders.m_pLightShader->Activate();
		m_shaders.m_pLightShader->SetMatrixFloat4("modelMatrix", lightModel);
		m_shaders.m_pLightShader->SetFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);
	}

	void OpenGLRendererAPI::RenderScene(const RenderData& _renderData)
	{
		// Enable settings
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);	
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		// Clear buffers before rendering
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Pre Render
		
		// Rendering Passes
		GeometryPass(_renderData);
		TerrainPass(_renderData);
		LightingPass(_renderData);
		
		SkyboxPass(_renderData);
		DebugPass();
			
		// Post Render

	}

	void OpenGLRendererAPI::GeometryPass(const RenderData& _renderData)
	{
		for (RenderItem3D renderItem : _renderData.vGeometryRenderItems)
		{
			Shared<Shader> gShader = renderItem.pMesh->GetBoneInfos().empty() ? m_shaders.m_pDefaultShader : m_shaders.m_pSkinningShader;

			// Light Params
			gShader->Activate();
			const Transform& cameraTransform = _renderData.pPlayerCamera->GetTransform();
			gShader->SetFloat3("camPos", cameraTransform.m_vWorldPosition.x, cameraTransform.m_vWorldPosition.y, cameraTransform.m_vWorldPosition.z);
			gShader->SetFloat3("spotLight.lightPos", _renderData.vSpotLights[0].vPosition.x, _renderData.vSpotLights[0].vPosition.y, _renderData.vSpotLights[0].vPosition.z);
			gShader->SetFloat("material.shininess", 256.0f);

			// Bone Transforms
			std::vector<BoneInfo> vBoneInfos = renderItem.pMesh->GetBoneInfos();
			int iSize = (int)vBoneInfos.size();
			for (int iIndex = 0; iIndex < iSize; iIndex++)
			{
				std::string strHandle = std::to_string(iIndex);
				gShader->SetMatrixFloat4(std::string("matrixPalette[" + strHandle + "]").c_str(), vBoneInfos[iIndex].finalTransform);
			}

			// Create Render Params
			RenderParams renderParams;
			renderParams.pCamera = _renderData.pPlayerCamera;
			renderParams.pShader = gShader;
			renderParams.drawMode = DrawMode::ELEMENTS_BASE_VERTEX;

			// Create Renderer
			MeshRenderer geometryRenderer;
			geometryRenderer.SetMesh(renderItem.pMesh);
			geometryRenderer.Render(renderParams);			
		}
	}

	void OpenGLRendererAPI::TerrainPass(const RenderData& _renderData)
	{
		// Bind Materials
		std::vector<Material> vMaterials = _renderData.pTerrain->GetMaterials();
		int iSize = (int)vMaterials.size();
		for (int i = 0; i < iSize; i++)
		{
			vMaterials[i].Bind(GL_TEXTURE0 + i);
		}
			
		// Create Render Params
		RenderParams renderParams;
		renderParams.pCamera = _renderData.pPlayerCamera;
		renderParams.pShader = m_shaders.m_pTesselationShader;
		renderParams.drawMode = DrawMode::ARRAYS;
		renderParams.bBindMaterial = false;

		// Create Renderer
		MeshRenderer meshRenderer;
		meshRenderer.SetMesh(_renderData.pTerrain);		
		meshRenderer.Render(renderParams);
	}

	void OpenGLRendererAPI::LightingPass(const RenderData& _renderData)
	{
		m_shaders.m_pDefaultShader->Activate();
		m_shaders.m_pDefaultShader->SetDirLight(_renderData.dirLight);
		m_shaders.m_pDefaultShader->SetPointLight(_renderData.vPointLights[0], 0);
		m_shaders.m_pDefaultShader->SetSpotLight(_renderData.vSpotLights[0], 0);

		m_shaders.m_pSkinningShader->Activate();
		m_shaders.m_pSkinningShader->SetDirLight(_renderData.dirLight);
		m_shaders.m_pSkinningShader->SetPointLight(_renderData.vPointLights[0], 0);
		m_shaders.m_pSkinningShader->SetSpotLight(_renderData.vSpotLights[0], 0);

		//m_lightShader.Activate();
		//glm::mat4 lightModel = glm::mat4(1.0f);
		//lightModel = glm::translate(lightModel, _renderData.vSpotLights[0].vPosition);
		//lightModel = glm::scale(lightModel, glm::vec3(0.25f, 0.25f, 0.25f));
		//m_lightShader.SetMatrixFloat4("modelMatrix", lightModel);
	}

	void OpenGLRendererAPI::SkyboxPass(const RenderData& _renderData)
	{
		// Camera
		glFrontFace(GL_CW);
		glDepthFunc(GL_LEQUAL);

		_renderData.pPlayerCamera->SetTranslation(false);

		// Create Render Params
		RenderParams renderParams;
		renderParams.pCamera = _renderData.pPlayerCamera;
		renderParams.pShader = m_shaders.m_pSkyboxShader;
		renderParams.drawMode = DrawMode::ELEMENTS_BASE_VERTEX;

		// Create Renderer
		MeshRenderer meshRenderer;
		meshRenderer.SetMesh(_renderData.pSkybox);
		meshRenderer.SetDefaultMaterial(Material(hashID(constants::pSyboxTextureName)));
		meshRenderer.Render(renderParams);

		_renderData.pPlayerCamera->SetTranslation(true);

		glDepthFunc(GL_LESS);
	}

	void OpenGLRendererAPI::DebugPass()
	{
		// Debug Primitives
		DebugDraw::DrawPrimitives(m_shaders.m_pDebugShader);

		// Debug Menu
		DebugMenu::Draw();
		
		// Render Dear ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void OpenGLRendererAPI::Clear()
	{
		m_shaders.m_pDebugShader->Delete();
		m_shaders.m_pDefaultShader->Delete();
		m_shaders.m_pLightShader->Delete();
		m_shaders.m_pSkinningShader->Delete();
		m_shaders.m_pSkyboxShader->Delete();
		m_shaders.m_pTesselationShader->Delete();
	}
}