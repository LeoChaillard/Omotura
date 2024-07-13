#include "AnimationImporter.h"

#include "../Common/Common.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Omotura
{
	Shared<Animation> AnimationImporter::LoadAnimation(const AssetInfo& _infos)
	{	
		Shared<Animation> pAnimation = CreateShared<Animation>();
		float fDuration = 0.0f;
		float fTicksPerSecond = 0.0f;
		float fFinalTimeStamp = 0.0f;

		Assimp::Importer importer;
		std::string strFilePath = std::get<std::string>(_infos.strFilePath);
		const aiScene* pScene = importer.ReadFile(strFilePath.c_str(), ASSIMP_LOAD_FLAGS);
		if (pScene)
		{
			aiAnimation* pAnim = pScene->mAnimations[0];
	
			fDuration = (float)pAnim->mDuration;
			fTicksPerSecond = (float)pAnim->mTicksPerSecond;

			// Retrieving the number of nodes/joints
			int iNodeCount = pAnim->mNumChannels;

			// Creating animation clip and filling it with poses
			for (int i = 0; i < iNodeCount; i++)
			{
				AnimatedNode animatedNode;
				int iNumPositionKeys = pAnim->mChannels[i]->mNumPositionKeys;
				for (int j = 0; j < iNumPositionKeys; j++)
				{
					aiVectorKey pos = pAnim->mChannels[i]->mPositionKeys[j];
					aiQuatKey rot = pAnim->mChannels[i]->mRotationKeys[j];
					aiVectorKey scale = pAnim->mChannels[i]->mScalingKeys[j];

					// Joint Transform
					Transform transform;
					transform.m_vLocalPosition = Vector3(pos.mValue.x, pos.mValue.y, pos.mValue.z);
					transform.m_quaternion = Quaternion(rot.mValue.w, Vector3(rot.mValue.x, rot.mValue.y, rot.mValue.z));
					transform.m_vLocalScale = Vector3(scale.mValue.x);

					// Key
					NodeKey nodeKey;
					nodeKey.fTimeStamp = (float)pAnim->mChannels[i]->mPositionKeys[j].mTime;
					nodeKey.boneTransform = transform;

					animatedNode.vNodeKeys.push_back(nodeKey);

					fFinalTimeStamp = std::max(fFinalTimeStamp, nodeKey.fTimeStamp);
				}				
				animatedNode.handle = hashID(pAnim->mChannels[i]->mNodeName.data);
				pAnimation->AddNode(animatedNode);
			}
		}
		else
		{
			printf("Error parsing '%s': '%s'\n", strFilePath.c_str(), importer.GetErrorString());
		}

		pAnimation->SetDuration(fDuration);
		pAnimation->SetTicksPerSecond(fTicksPerSecond);
		pAnimation->SetFinalTimeStamp(fFinalTimeStamp);

		importer.FreeScene();

		return pAnimation;
	}
}