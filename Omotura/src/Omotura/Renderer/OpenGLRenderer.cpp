#include "OpenGLRenderer.h"
#include "RendererCommon.h"
#include "Texture.h"

#include "../Asset/AssetManager.h"
#include "../Utils/Utils.hpp"

#include <glm/glm.hpp>

namespace Omotura
{
	namespace constants
	{
		const char* pNoTextureName = "NoTexture";
	}

	OpenGLRenderer::OpenGLRenderer()
		: m_shaderProgram(),
		m_lightShader(),
		m_skyboxShader(),
		m_cubemapTexture()
	{
		Init();
	}

	void OpenGLRenderer::Init()
	{
		// Load Assets
		std::string strResourcesFolder = Utils::GetResourcesFolder();
		std::string strModelFolder = strResourcesFolder + "/models/";
		std::string strTexturesFolder = strResourcesFolder + "/textures/";
		std::string strShaderFolder = strResourcesFolder + "/shaders/";

		// Shaders
		m_shaderProgram = Shader((strShaderFolder + "default.vert").c_str(), (strShaderFolder + "default.frag").c_str());
		m_skinningShader = Shader((strShaderFolder + "skinning.vert").c_str(), (strShaderFolder + "skinning.frag").c_str());
		m_lightShader = Shader((strShaderFolder + "light.vert").c_str(), (strShaderFolder + "light.frag").c_str());
		m_skyboxShader = Shader((strShaderFolder + "skybox.vert").c_str(), (strShaderFolder + "skybox.frag").c_str());

		// Light shader
		glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 2.0f);
		glm::vec3 lightColor = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.25f, 0.25f, 0.25f));

		m_lightShader.Activate();
		m_lightShader.SetMatrixFloat4("modelMatrix", lightModel);
		m_lightShader.SetFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);		
	}

	void OpenGLRenderer::RenderGame(const RenderData& _renderData)
	{
		// Enable settings
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);	
		//glCullFace(GL_FRONT);
		//glFrontFace(GL_CCW);
		
		// Clear buffers before rendering
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Pre Render

		// Rendering Passes
		GeometryPass(_renderData);
		LightingPass(_renderData);

		SkyboxPass(_renderData);
		DebugPass();

		// Post Render

	}

	void OpenGLRenderer::GeometryPass(const RenderData& _renderData)
	{
		for (SkinnedRenderItem3D skinnedRenderItem : _renderData.vGeometrySkinnedRenderItems)
		{
			m_skinningShader.Activate();
			m_skinningShader.SetMatrixFloat4("modelMatrix", skinnedRenderItem.modelMatrix);
			m_skinningShader.SetFloat3("spotLight.lightPos", _renderData.vSpotLights[0].vPosition.x, _renderData.vSpotLights[0].vPosition.y, _renderData.vSpotLights[0].vPosition.z);

			int iIndex = 0;
			for (const glm::mat4& boneTranform : skinnedRenderItem.vBoneTransforms)
			{
				std::string strHandle = std::to_string(iIndex);
				m_skinningShader.SetMatrixFloat4(std::string("matrixPalette[" + strHandle + "]").c_str(), boneTranform);
				iIndex++;
			}

			// Bind
			glBindVertexArray(skinnedRenderItem.skinnedModel->GetVAO());

			// Draw
			int iMeshes = skinnedRenderItem.skinnedModel->GetMeshes().size();
			for (unsigned int j = 0; j < iMeshes; j++)
			{
				unsigned int iMaterialIndex = skinnedRenderItem.skinnedModel->GetMeshes()[j].iMaterialIndex;

				Shared<Texture> pDiffuse, pNormal, pRMA;
				if (iMaterialIndex != INVALID_MATERIAL)
				{
					assert(iMaterialIndex < skinnedRenderItem.skinnedModel->GetMaterials().size());

					pDiffuse = AssetManager::GetAsset<Texture>(skinnedRenderItem.skinnedModel->GetMaterials()[iMaterialIndex].diffuseHandle);
					if (pDiffuse)
					{
						m_skinningShader.Activate();
						m_skinningShader.SetInt("material.diffuse0", TEXTURE_UNIT_DIFFUSE);
						m_skinningShader.SetFloat("material.shininess", 256.0f);
						pDiffuse->Bind(TEXTURE_UNIT_DIFFUSE);

						pNormal = AssetManager::GetAsset<Texture>(skinnedRenderItem.skinnedModel->GetMaterials()[iMaterialIndex].normalHandle);
						if (pNormal)
						{
							m_skinningShader.Activate();
							m_skinningShader.SetInt("material.normal0", TEXTURE_UNIT_NORMAL);
							pNormal->Bind(TEXTURE_UNIT_NORMAL);
						}

						pRMA = AssetManager::GetAsset<Texture>(skinnedRenderItem.skinnedModel->GetMaterials()[iMaterialIndex].rmaHandle);
						if (pRMA)
						{
							m_skinningShader.Activate();
							m_skinningShader.SetInt("material.specular0", TEXTURE_UNIT_RMA);
							pRMA->Bind(TEXTURE_UNIT_RMA);
						}
					}
					else
					{
						pDiffuse = AssetManager::GetAsset<Texture>(hashID(constants::pNoTextureName));
						m_skinningShader.Activate();
						m_skinningShader.SetInt("material.diffuse0", TEXTURE_UNIT_DIFFUSE);
						m_skinningShader.SetFloat("material.shininess", 256.0f);
						pDiffuse->Bind(TEXTURE_UNIT_DIFFUSE);
					}
				}

				// Camera
				Shared<Camera> pCamera = _renderData.pPlayerCamera;
				const Transform& cameraTransform = pCamera->GetTransform();
				m_skinningShader.SetFloat3("camPos", cameraTransform.m_vWorldPosition.x, cameraTransform.m_vWorldPosition.y, cameraTransform.m_vWorldPosition.z);
				m_skinningShader.SetMatrixFloat4("viewMatrix", pCamera->GetViewMatrix());
				m_skinningShader.SetMatrixFloat4("projectionMatrix", pCamera->GetPerspectiveMatrix());

				// Draw
				glDrawElementsBaseVertex(GL_TRIANGLES,
					skinnedRenderItem.skinnedModel->GetMeshes()[j].iNumIndices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(GLuint) * skinnedRenderItem.skinnedModel->GetMeshes()[j].iBaseIndex),
					skinnedRenderItem.skinnedModel->GetMeshes()[j].iBaseVertex);

				// Unbind textures
				if (pDiffuse) pDiffuse->Unbind();
				if (pNormal) pNormal->Unbind();
				if (pRMA) pRMA->Unbind();
			}

			glBindVertexArray(0);
		}

		for (RenderItem3D renderItem : _renderData.vGeometryRenderItems)
		{
			m_shaderProgram.Activate();
			m_shaderProgram.SetMatrixFloat4("modelMatrix", renderItem.modelMatrix);
			m_shaderProgram.SetFloat3("spotLight.lightPos", _renderData.vSpotLights[0].vPosition.x, _renderData.vSpotLights[0].vPosition.y, _renderData.vSpotLights[0].vPosition.z);

			// Bind
			glBindVertexArray(renderItem.model->GetVAO());

			// Draw
			int iMeshes = renderItem.model->GetMeshes().size();
			for (unsigned int j = 0; j < iMeshes; j++)
			{
				unsigned int iMaterialIndex = renderItem.model->GetMeshes()[j].iMaterialIndex;

				Shared<Texture> pDiffuse, pNormal, pRMA;
				if (iMaterialIndex != INVALID_MATERIAL)
				{
					assert(iMaterialIndex < renderItem.model->GetMaterials().size());

					pDiffuse = AssetManager::GetAsset<Texture>(renderItem.model->GetMaterials()[iMaterialIndex].diffuseHandle);
					if (pDiffuse)
					{
						m_shaderProgram.Activate();
						m_shaderProgram.SetInt("material.diffuse0", TEXTURE_UNIT_DIFFUSE);
						m_shaderProgram.SetFloat("material.shininess", 256.0f);
						pDiffuse->Bind(TEXTURE_UNIT_DIFFUSE);

						pNormal = AssetManager::GetAsset<Texture>(renderItem.model->GetMaterials()[iMaterialIndex].normalHandle);
						if (pNormal)
						{
							m_shaderProgram.Activate();
							m_shaderProgram.SetInt("material.normal0", TEXTURE_UNIT_NORMAL);
							pNormal->Bind(TEXTURE_UNIT_NORMAL);
						}

						pRMA = AssetManager::GetAsset<Texture>(renderItem.model->GetMaterials()[iMaterialIndex].rmaHandle);
						if (pRMA)
						{
							m_shaderProgram.Activate();
							m_shaderProgram.SetInt("material.specular0", TEXTURE_UNIT_RMA);
							pRMA->Bind(TEXTURE_UNIT_RMA);
						}
					}
					else
					{
						pDiffuse = AssetManager::GetAsset<Texture>(hashID(constants::pNoTextureName));
						m_shaderProgram.Activate();
						m_shaderProgram.SetInt("material.diffuse0", TEXTURE_UNIT_DIFFUSE);
						m_shaderProgram.SetFloat("material.shininess", 256.0f);
						pDiffuse->Bind(TEXTURE_UNIT_DIFFUSE);
					}
				}

				// Camera
				Shared<Camera> pCamera = _renderData.pPlayerCamera;
				const Transform& cameraTransform = pCamera->GetTransform();
				m_skinningShader.SetFloat3("camPos", cameraTransform.m_vWorldPosition.x, cameraTransform.m_vWorldPosition.y, cameraTransform.m_vWorldPosition.z);
				m_shaderProgram.SetMatrixFloat4("viewMatrix", pCamera->GetViewMatrix());
				m_shaderProgram.SetMatrixFloat4("projectionMatrix", pCamera->GetPerspectiveMatrix());

				// Draw
				glDrawElementsBaseVertex(GL_TRIANGLES,
					renderItem.model->GetMeshes()[j].iNumIndices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(GLuint) * renderItem.model->GetMeshes()[j].iBaseIndex),
					renderItem.model->GetMeshes()[j].iBaseVertex);

				// Unbind textures
				if (pDiffuse) pDiffuse->Unbind();
				if (pNormal) pNormal->Unbind();
				if (pRMA) pRMA->Unbind();
			}

			glBindVertexArray(0);
		}
	}

	void OpenGLRenderer::LightingPass(const RenderData& _renderData)
	{
		m_shaderProgram.Activate();
		m_shaderProgram.SetDirLight(_renderData.dirLight);
		m_shaderProgram.SetPointLight(_renderData.vPointLights[0], 0);
		m_shaderProgram.SetSpotLight(_renderData.vSpotLights[0], 0);

		m_skinningShader.Activate();
		m_skinningShader.SetDirLight(_renderData.dirLight);
		m_skinningShader.SetPointLight(_renderData.vPointLights[0], 0);
		m_skinningShader.SetSpotLight(_renderData.vSpotLights[0], 0);

		//m_lightShader.Activate();
		//glm::mat4 lightModel = glm::mat4(1.0f);
		//lightModel = glm::translate(lightModel, _renderData.vSpotLights[0].vPosition);
		//lightModel = glm::scale(lightModel, glm::vec3(0.25f, 0.25f, 0.25f));
		//m_lightShader.SetMatrixFloat4("modelMatrix", lightModel);
	}

	void OpenGLRenderer::SkyboxPass(const RenderData& _renderData)
	{
		// Camera
		glDepthFunc(GL_LEQUAL);

		m_skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = _renderData.pPlayerCamera->GetOrientation() * glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)1920 / 1080, 0.1f, 100.0f);
		m_skyboxShader.SetMatrixFloat4("view", view);
		m_skyboxShader.SetMatrixFloat4("projection", projection);

		// Bind
		glBindVertexArray(_renderData.pSkyboxModel->GetVAO());

		m_skyboxShader.Activate();
		m_skyboxShader.SetInt("skybox", GL_TEXTURE0);
		AssetManager::GetAsset<Texture>(hashID("NightSky"))->Bind(GL_TEXTURE0);

		// Draw
		glDrawElementsBaseVertex(GL_TRIANGLES,
			_renderData.pSkyboxModel->GetMeshes()[0].iNumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(GLuint) * _renderData.pSkyboxModel->GetMeshes()[0].iBaseIndex),
			_renderData.pSkyboxModel->GetMeshes()[0].iBaseVertex);

		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

	void OpenGLRenderer::DebugPass()
	{
		// Debug Menu

		// Draw Debug Primitives

	}

	void OpenGLRenderer::Clear()
	{
		m_shaderProgram.Delete();
		m_lightShader.Delete();
		m_skyboxShader.Delete();
	}
}