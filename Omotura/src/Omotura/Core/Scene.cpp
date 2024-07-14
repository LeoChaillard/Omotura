#include "Scene.h"

#include "../Asset/AssetManager.h"

namespace Omotura
{
	Scene::Scene()
	{
		// Models
		m_vpSkinnedModels.push_back(AssetManager::GetAsset<SkinnedModel>(hashID("AKS74U")));
		m_vpSkinnedModels.push_back(AssetManager::GetAsset<SkinnedModel>(hashID("Glock")));
		m_vpSkinnedModels.push_back(AssetManager::GetAsset<SkinnedModel>(hashID("Tokarev")));
		m_vpModels.push_back(AssetManager::GetAsset<Model>(hashID("House")));

		// Models transforms
		glm::mat4 objectModel = glm::mat4(1.0f);
		glm::mat4 mTranslate  = glm::translate(glm::vec3(2.0f, 2.0f, 5.0f));
		glm::mat4 mRot = glm::mat4(1.0f);
		glm::mat4 mScale = glm::scale(glm::vec3(0.01f));
		objectModel = mTranslate * mScale * objectModel;
		m_vpSkinnedModels[0]->SetModelMatrix(objectModel);
		m_vpSkinnedModels[0]->UpdateBoneTransformsFromBindPose();
		m_vpSkinnedModels[0]->SetAnimation("AKS74U_Reload");

		objectModel = glm::mat4(1.0f);
		mTranslate = glm::translate(glm::vec3(0.0f, 2.0f, 0.0f));
		mRot = glm::mat4(1.0f);
		mScale = glm::scale(glm::vec3(0.01f));
		objectModel = mTranslate * mRot * mScale * objectModel;
		m_vpSkinnedModels[1]->SetModelMatrix(objectModel);
		m_vpSkinnedModels[1]->UpdateBoneTransformsFromBindPose();
		m_vpSkinnedModels[1]->SetAnimation("Glock_Idle");

		objectModel = glm::mat4(1.0f);
		mTranslate = glm::translate(glm::vec3(-2.0f, 2.0f, 5.0f));
		mRot = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mScale = glm::scale(glm::vec3(0.01f));
		objectModel = mTranslate * mRot * mScale * objectModel;
		m_vpSkinnedModels[2]->SetModelMatrix(objectModel);
		m_vpSkinnedModels[2]->UpdateBoneTransformsFromBindPose();
		m_vpSkinnedModels[2]->SetAnimation("Tokarev_Reload");

		objectModel = glm::mat4(1.0f);
		mTranslate = glm::translate(glm::vec3(0.0, 0.0f, 0.0f));
		mRot = glm::rotate(glm::radians(-90.0f), glm::vec3(1.0, 0.0f, 0.0f));
		mScale = glm::scale(glm::vec3(1.0f));
		objectModel = mTranslate * mRot * mScale * objectModel;
		m_vpModels[0]->SetModelMatrix(objectModel);
	}

	void Scene::Update(float _fDeltaTime)
	{
		// Update skinning matrices
		int iSkins = m_vpSkinnedModels.size();
		m_vAllBoneTransforms.resize(iSkins);
		for (int i = 0; i < iSkins; i++)
		{
			std::vector<glm::mat4> vBoneTransforms;
			m_vpSkinnedModels[i]->GetBoneTransforms(_fDeltaTime, vBoneTransforms);
			m_vAllBoneTransforms[i] = vBoneTransforms;
		}
	}

	const std::vector<Shared<SkinnedModel>>& Scene::GetSkinnedModels() const
	{
		return m_vpSkinnedModels;
	}

	const std::vector<Shared<Model>>& Scene::GetModels() const
	{
		return m_vpModels;
	}

	const std::vector<std::vector<glm::mat4>>& Scene::GetAllBoneTransforms() const
	{
		return m_vAllBoneTransforms;
	}
}