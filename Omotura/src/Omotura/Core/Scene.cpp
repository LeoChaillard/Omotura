#include "Scene.h"
#include "Game.h"

#include "../Asset/AssetManager.h"

namespace Omotura
{
	extern Game g_gameInstance;

	Scene::Scene()
	{
		m_pTerrain = CreateShared<Terrain>();
	}

	void Scene::Init()
	{
		// Meshes
		m_vpMeshes.push_back(AssetManager::GetAsset<Mesh>(hashID("AKS74U")));
		m_vpMeshes.push_back(AssetManager::GetAsset<Mesh>(hashID("Glock")));
		m_vpMeshes.push_back(AssetManager::GetAsset<Mesh>(hashID("Tokarev")));
		m_vpMeshes.push_back(AssetManager::GetAsset<Mesh>(hashID("House")));

		// Meshes transforms
		glm::mat4 objectModel = glm::mat4(1.0f);
		glm::mat4 mTranslate = glm::translate(glm::vec3(0.0f, 2.0f, 0.0f));
		glm::mat4 mRot = glm::mat4(1.0f);
		glm::mat4 mScale = glm::scale(glm::vec3(0.01f));
		objectModel = mTranslate * mScale * objectModel;
		m_vpMeshes[0]->SetModelMatrix(objectModel);

		objectModel = glm::mat4(1.0f);
		mTranslate = glm::translate(glm::vec3(0.0f, 2.0f, 0.0f));
		mRot = glm::rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mScale = glm::scale(glm::vec3(0.01f));
		objectModel = mTranslate * mRot * mScale * objectModel;
		m_vpMeshes[1]->SetModelMatrix(objectModel);
		m_vpMeshes[1]->Hide();

		objectModel = glm::mat4(1.0f);
		mTranslate = glm::translate(glm::vec3(-2.0f, 2.0f, 5.0f));
		mRot = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mScale = glm::scale(glm::vec3(0.01f));
		objectModel = mTranslate * mRot * mScale * objectModel;
		m_vpMeshes[2]->SetModelMatrix(objectModel);
		m_vpMeshes[2]->Hide();

		objectModel = glm::mat4(1.0f);
		mTranslate = glm::translate(glm::vec3(0.0, 0.0f, 0.0f));
		mRot = glm::rotate(glm::radians(-90.0f), glm::vec3(1.0, 0.0f, 0.0f));
		mScale = glm::scale(glm::vec3(1.0f));
		objectModel = mTranslate * mRot * mScale * objectModel;
		m_vpMeshes[3]->SetModelMatrix(objectModel);
		m_vpMeshes[3]->Hide();

		// Terrain
		m_pTerrain->Init();
	}

	void Scene::Update(float _fDeltaTime)
	{
		// Update matrix palettes
		int iMeshes = (int)m_vpMeshes.size();
		for (int i = 0; i < iMeshes; i++)
		{
			m_vpMeshes[i]->UpdateMatrixPalette(g_gameInstance.GetPlayer()->GetAnimator(), _fDeltaTime);
		}
	}

	const std::vector<Shared<Mesh>>& Scene::GetMeshes() const
	{
		return m_vpMeshes;

	}
	Shared<Terrain> Scene::GetTerrain() const
	{
		return m_pTerrain;
	}
}