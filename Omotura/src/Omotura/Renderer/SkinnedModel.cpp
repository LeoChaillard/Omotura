#include "SkinnedModel.h"

#include "../Asset/AssetImporter.h"
#include "../Asset/AssetManager.h"
#include "../API/OpenGLTexture.h"
#include "../Renderer/RendererCommon.h"
#include "../Utils/Math.hpp"
#include "../Core/Game.h"

namespace Omotura
{
	SkinnedModel::SkinnedModel(const std::string& _strFilePath)
	{
		m_pCurrentAnimation = CreateShared<Animation>();
		LoadSkinnedModel(_strFilePath);
	}

	SkinnedModel::~SkinnedModel()
	{
		m_importer.FreeScene();
	}

	bool SkinnedModel::LoadSkinnedModel(const std::string& _strFilePath)
	{
		// Creating VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// Generating buffers for the vertices attributes
		glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);

		// Importing scene
		bool bSuccess = false;

		//m_importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		m_pScene = m_importer.ReadFile(_strFilePath.c_str(), ASSIMP_LOAD_FLAGS);
		if (m_pScene)
		{
			bSuccess = InitFromScene(m_pScene, _strFilePath);
		}
		else
		{
			printf("Error parsing '%s': '%s'\n", _strFilePath.c_str(), m_importer.GetErrorString());
		}

		// Unbinding VAO
		glBindVertexArray(0);

		return bSuccess;
	}

	bool SkinnedModel::InitFromScene(const aiScene* _pScene, const std::string _strFileName)
	{
		m_vMeshes.resize(_pScene->mNumMeshes);
		m_vMaterials.resize(_pScene->mNumMaterials);
		m_globalInverseTransform = glm::inverse(AiMatrix4x4ToGlm(_pScene->mRootNode->mTransformation));

		unsigned int iNumVertices = 0;
		unsigned int iNumIndices = 0;

		CountVerticesAndIndices(_pScene, iNumVertices, iNumIndices);

		ReserveSpace(iNumVertices, iNumIndices);

		InitAllMeshes(_pScene);

		if (!InitMaterials(_pScene, _strFileName))
		{
			return false;
		}

		PopulateBuffers();

		return true;
	}

	void SkinnedModel::CountVerticesAndIndices(const aiScene* _pScene, unsigned int& _iNumVertices, unsigned int& _iNumIndices)
	{
		for (unsigned int i = 0; i < m_vMeshes.size(); i++)
		{
			m_vMeshes[i].iMaterialIndex = _pScene->mMeshes[i]->mMaterialIndex;
			m_vMeshes[i].iNumIndices = _pScene->mMeshes[i]->mNumFaces * 3;
			m_vMeshes[i].iBaseVertex = _iNumVertices;
			m_vMeshes[i].iBaseIndex = _iNumIndices;

			_iNumVertices += _pScene->mMeshes[i]->mNumVertices;
			_iNumIndices += m_vMeshes[i].iNumIndices;
		}
	}

	void SkinnedModel::ReserveSpace(unsigned int _iNumVertices, unsigned int _iNumIndices)
	{
		m_vPositions.reserve(_iNumVertices);
		m_vNormals.reserve(_iNumVertices);
		m_vTextCoords.reserve(_iNumVertices);
		m_vIndices.reserve(_iNumIndices);

		m_vBoneIDs.resize(_iNumVertices);
		m_vWeights.resize(_iNumVertices);
	}

	void SkinnedModel::InitAllMeshes(const aiScene* _pScene)
	{
		int iSize = m_vMeshes.size();
		for (int i = 0; i < iSize; i++)
		{
			const aiMesh* paiMesh = _pScene->mMeshes[i];
			InitSingleMesh(paiMesh, m_vMeshes[i].iBaseVertex);
		}

		// Get skeleton joints/nodes
		RetrieveSkeletonJoints(_pScene->mRootNode, -1);
	}

	void SkinnedModel::InitSingleMesh(const aiMesh* _pMesh, int _iBaseVertex)
	{
		const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

		// Populate the vertex attribute vectors
		int iVertices = _pMesh->mNumVertices;
		for (int i = 0; i < iVertices; i++)
		{
			const aiVector3D& pos = _pMesh->mVertices[i];
			const aiVector3D& normal = _pMesh->mNormals[i];
			const aiVector3D& textCoord = _pMesh->HasTextureCoords(0) ? _pMesh->mTextureCoords[0][i] : zero3D;

			m_vPositions.push_back(glm::vec3(pos.x, pos.y, pos.z));
			m_vNormals.push_back(glm::vec3(normal.x, normal.y, normal.z));
			m_vTextCoords.push_back(glm::vec2(textCoord.x, textCoord.y));
		}

		// Retrieve bones
		RetrieveBones(_pMesh, _iBaseVertex);

		// Populate the index buffer
		int iFaces = _pMesh->mNumFaces;
		for (int i = 0; i < iFaces; i++)
		{
			const aiFace& face = _pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			m_vIndices.push_back(face.mIndices[0]);
			m_vIndices.push_back(face.mIndices[1]);
			m_vIndices.push_back(face.mIndices[2]);
		}
	}

	bool SkinnedModel::InitMaterials(const aiScene* _pScene, const std::string& _strFileName)
	{
		// Initialize the materials
		int iMaterials = _pScene->mNumMaterials;
		for (int i = 0; i < iMaterials; i++)
		{
			const aiMaterial* pMaterial = _pScene->mMaterials[i];

			aiString materialName = pMaterial->GetName();
			std::string strName(materialName.C_Str());

			m_vMaterials[i].diffuseHandle = hashID((strName + std::string("_ALB")).c_str());
			m_vMaterials[i].normalHandle = hashID((strName + std::string("_NRM")).c_str());
			m_vMaterials[i].rmaHandle = hashID((strName + std::string("_RMA")).c_str());
		}
		return true;
	}

	void SkinnedModel::PopulateBuffers()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vIndices[0]) * m_vIndices.size(), m_vIndices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vPositions[0]) * m_vPositions.size(), m_vPositions.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(ATTRIBUTE_POSITION_LOCATION);
		glVertexAttribPointer(ATTRIBUTE_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vNormals[0]) * m_vNormals.size(), m_vNormals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(ATTRIBUTE_NORMAL_LOCATION);
		glVertexAttribPointer(ATTRIBUTE_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vTextCoords[0]) * m_vTextCoords.size(), m_vTextCoords.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(ATTRIBUTE_TEX_COORD_LOCATION);
		glVertexAttribPointer(ATTRIBUTE_TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_ID_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vBoneIDs[0]) * m_vBoneIDs.size(), m_vBoneIDs.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(ATTRIBUTE_BONE_ID_LOCATION);
		glVertexAttribIPointer(ATTRIBUTE_BONE_ID_LOCATION, 4, GL_INT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_WEIGHT_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vWeights[0]) * m_vWeights.size(), m_vWeights.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(ATTRIBUTE_BONE_WEIGHT_LOCATION);
		glVertexAttribPointer(ATTRIBUTE_BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}

	GLuint SkinnedModel::GetVAO()
	{
		return m_VAO;
	}

	const std::vector<Mesh>& SkinnedModel::GetMeshes() const
	{
		return m_vMeshes;
	}

	const std::vector<Material>& SkinnedModel::GetMaterials() const
	{
		return m_vMaterials;
	}

	void SkinnedModel::RetrieveBones(const aiMesh* _pMesh, int _iBaseVertex)
	{
		// Load infos
		int iSize = _pMesh->mNumBones;
		for (int i = 0; i < iSize; i++)
		{
			aiBone* pBone = _pMesh->mBones[i];
			std::string strBoneName = pBone->mName.C_Str();
			BoneHandle boneHandle = hashID(strBoneName.c_str());

			// Create bone if doesn't exist (may be shared between meshes)
			if (m_nodeBoneMap.find(boneHandle) == m_nodeBoneMap.end())
			{
				BoneInfo boneInfo;
				boneInfo.handle = boneHandle;
				boneInfo.offsetMatrix = AiMatrix4x4ToGlm(_pMesh->mBones[i]->mOffsetMatrix);
				m_nodeBoneMap[boneHandle] = m_vBonesInfos.size();
				m_vBonesInfos.push_back(boneInfo);
			}

			// Retrieve weights and bone IDs
			int iWeights = pBone->mNumWeights;
			for (int j = 0; j < iWeights; j++)
			{
				int iBoneIndex = m_nodeBoneMap[boneHandle];
				const aiVertexWeight& vertexWeight = pBone->mWeights[j];
				int iGlobalVertexID = _iBaseVertex + vertexWeight.mVertexId;
				if (m_vWeights[iGlobalVertexID].x == 0)
				{
					//if (i == 1) std::cout << strBoneName << ": " << vertexWeight.mWeight << std::endl;
					m_vBoneIDs[iGlobalVertexID].x = iBoneIndex;
					m_vWeights[iGlobalVertexID].x = vertexWeight.mWeight;
				}
				else if (m_vWeights[iGlobalVertexID].y == 0)
				{
					m_vBoneIDs[iGlobalVertexID].y = iBoneIndex;
					m_vWeights[iGlobalVertexID].y = vertexWeight.mWeight;
				}
				else if (m_vWeights[iGlobalVertexID].z == 0)
				{
					m_vBoneIDs[iGlobalVertexID].z = iBoneIndex;
					m_vWeights[iGlobalVertexID].z = vertexWeight.mWeight;
				}
				else if (m_vWeights[iGlobalVertexID].w == 0)
				{
					m_vBoneIDs[iGlobalVertexID].w = iBoneIndex;
					m_vWeights[iGlobalVertexID].w = vertexWeight.mWeight;
				}
			}
		}
	}

	void SkinnedModel::RetrieveSkeletonJoints(const aiNode* _pNode, int _iParentIndex)
	{
		Joint joint;
		joint.handle = hashID(_pNode->mName.data);
		joint.parentIndex = _iParentIndex; // Need to keep track of parent as it is recursive
		joint.nodeTransform = AiMatrix4x4ToGlm(_pNode->mTransformation);

		_iParentIndex = m_vJoints.size();
		m_vJoints.push_back(joint);

		int iChildren = _pNode->mNumChildren;
		for (int i = 0; i < iChildren; i++)
		{
			RetrieveSkeletonJoints(_pNode->mChildren[i], _iParentIndex);
		}
	}

	void SkinnedModel::UpdateBoneTransformsFromBindPose()
	{
		int iJoints = m_vJoints.size();
		for (int i = 0; i < iJoints; i++)
		{
			// Get the node and its transform matrix
			NodeHandle nodeHandle = m_vJoints[i].handle;
			glm::mat4 nodeTransform = m_vJoints[i].nodeTransform;

			int iParentIndex = m_vJoints[i].parentIndex;

			glm::mat4 parentTransform = (iParentIndex == -1) ? glm::mat4(1) : m_vJoints[iParentIndex].globalTransform;
			glm::mat4 globalTransform = parentTransform * nodeTransform;

			m_vJoints[i].globalTransform = globalTransform;

			if (m_nodeBoneMap.find(nodeHandle) != m_nodeBoneMap.end())
			{
				int iBoneIndex = m_nodeBoneMap[nodeHandle];
				m_vBonesInfos[iBoneIndex].finalTransform = m_globalInverseTransform * globalTransform * m_vBonesInfos[iBoneIndex].offsetMatrix;
				m_vBonesInfos[iBoneIndex].modelSpaceTransform = globalTransform;
			}
		}
	}

	void SkinnedModel::UpdateBoneTransformsFromAnimation()
	{
		float fAnimationTime = GetAnimationTime(m_fCurrentAnimationTime, m_pCurrentAnimation);
		int iJoints = m_vJoints.size();
		for (int i = 0; i < iJoints; i++)
		{
			// Get the node and its transform matrix
			NodeHandle nodeHandle = m_vJoints[i].handle;
			glm::mat4 nodeTransform = m_vJoints[i].nodeTransform;

			// Make animation transitions
			Shared<AnimatedNode> pAnimatedNode = FindAnimatedNode(m_pCurrentAnimation, nodeHandle);
			if (pAnimatedNode)
			{
				//glm::vec3 vScaling;
				//CalcInterpolatedScaling(vScaling, fAnimationTime, pAnimatedNode);
				//glm::mat4 scalingM =  glm::scale(glm::mat4(1.0f), vScaling);
				glm::mat4 scalingM = glm::mat4(1.0f);

				glm::quat qRotation;
				CalcInterpolatedRotation(qRotation, fAnimationTime, pAnimatedNode);
				glm::mat4 RotationM(qRotation);

				glm::vec3 vTranlation;
				CalcInterpolatedPosition(vTranlation, fAnimationTime, pAnimatedNode);

				glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), vTranlation);

				nodeTransform = TranslationM * RotationM * scalingM;
			}

			int iParentIndex = m_vJoints[i].parentIndex;

			glm::mat4 parentTransform = (iParentIndex == -1) ? glm::mat4(1) : m_vJoints[iParentIndex].globalTransform;
			glm::mat4 globalTransform = parentTransform * nodeTransform;

			m_vJoints[i].globalTransform = globalTransform;

			if (m_nodeBoneMap.find(nodeHandle) != m_nodeBoneMap.end())
			{
				int iBoneIndex = m_nodeBoneMap[nodeHandle];
				m_vBonesInfos[iBoneIndex].finalTransform = m_globalInverseTransform * globalTransform * m_vBonesInfos[iBoneIndex].offsetMatrix;
				m_vBonesInfos[iBoneIndex].modelSpaceTransform = globalTransform;
			}
		}
	}

	void SkinnedModel::GetBoneTransforms(float _fTimeInSeconds, std::vector<glm::mat4>& _transforms)
	{
		UpdateAnimation(_fTimeInSeconds);
		UpdateBoneTransformsFromAnimation();

		int iSize = m_vBonesInfos.size();
		_transforms.resize(iSize);
		for (int i = 0; i < iSize; i++)
		{
			_transforms[i] = m_vBonesInfos[i].finalTransform;
		}
	}

	void SkinnedModel::UpdateAnimation(float _fDeltaTime)
	{
		float fDuration = m_pCurrentAnimation->GetDuration() / m_pCurrentAnimation->GetTicksPerSecond();

		// Increase the animation time
		if (!m_pCurrentAnimation->IsPaused())
		{
			m_fCurrentAnimationTime += _fDeltaTime * m_pCurrentAnimation->GetSpeed();
		}
		// Animation is complete?
		if (m_fCurrentAnimationTime > fDuration)
		{
			if (!m_pCurrentAnimation->IsLooping())
			{
				m_fCurrentAnimationTime = fDuration;
				m_pCurrentAnimation->SetPaused(true);
			}
			else
			{
				m_fCurrentAnimationTime = 0.0f;
			}
		}
	}

	Shared<AnimatedNode> SkinnedModel::FindAnimatedNode(Shared<Animation> _pAnimation, BoneHandle _handle)
	{
		const std::vector<AnimatedNode>& vNodes = m_pCurrentAnimation->GetAnimatedNodes();
		int iNodes = vNodes.size();
		for (int i = 0; i < iNodes; i++)
		{
			if (vNodes[i].handle == _handle)
			{
				Shared<AnimatedNode> pNode = CreateShared<AnimatedNode>(vNodes[i]);
				return pNode;
			}
		}
	}

	int SkinnedModel::FindAnimatedNodeIndex(float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
	{
		// bail if current animation time is earlier than the this nodes first keyframe time
		if (_fAnimationTime < _pAnimatedNode->vNodeKeys[0].fTimeStamp)
			return -1; // you WERE returning -1 here

		for (unsigned int i = 1; i < _pAnimatedNode->vNodeKeys.size(); i++)
		{
			if (_fAnimationTime < _pAnimatedNode->vNodeKeys[i].fTimeStamp)
			{
				return i - 1;
			}
		}
		return (int)_pAnimatedNode->vNodeKeys.size() - 1;
	}

	float SkinnedModel::GetAnimationTime(float _fCurrentAnimationTime, Shared<Animation> _pCurrentAnimation)
	{
		float fTicksPerSecond = _pCurrentAnimation->GetTicksPerSecond() != 0 ? _pCurrentAnimation->GetTicksPerSecond() : 25.0f;
		float fTimeInTicks = _fCurrentAnimationTime * fTicksPerSecond;
		float fAnimationTime = fmod(fTimeInTicks, _pCurrentAnimation->GetDuration());
		return std::min(fTimeInTicks, _pCurrentAnimation->GetDuration());
	}

	void SkinnedModel::CalcInterpolatedScaling(glm::vec3& _vScaling, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
	{

		int iIndex = FindAnimatedNodeIndex(_fAnimationTime, _pAnimatedNode);
		int iNextIndex = iIndex + 1;

		// Discard if next frame out of range
		if (iNextIndex == _pAnimatedNode->vNodeKeys.size())
		{
			_vScaling = glm::vec3(_pAnimatedNode->vNodeKeys[iIndex].boneTransform.m_vLocalScale.x);
			return;
		}

		// Check if there is at least 2 frames
		if (iIndex == -1 || _pAnimatedNode->vNodeKeys.size() == 1)
		{
			_vScaling = glm::vec3(_pAnimatedNode->vNodeKeys[0].boneTransform.m_vLocalScale.x);
			return;
		}

		// Interpolating frames
		float fDeltaTime = _pAnimatedNode->vNodeKeys[iNextIndex].fTimeStamp - _pAnimatedNode->vNodeKeys[iIndex].fTimeStamp;
		float fFactor = (_fAnimationTime - _pAnimatedNode->vNodeKeys[iIndex].fTimeStamp) / fDeltaTime;

		Vector3 vStart = _pAnimatedNode->vNodeKeys[iIndex].boneTransform.m_vLocalScale;
		Vector3 vEnd = _pAnimatedNode->vNodeKeys[iNextIndex].boneTransform.m_vLocalScale;
		Vector3 vDelta = vEnd - vStart;

		_vScaling = vStart.ToGLM() + fFactor * vDelta.ToGLM();
	}

	void SkinnedModel::CalcInterpolatedRotation(glm::quat& _qRotation, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
	{
		int iIndex = FindAnimatedNodeIndex(_fAnimationTime, _pAnimatedNode);
		int iNextIndex = iIndex + 1;

		// Discard if next frame out of range
		if (iNextIndex == _pAnimatedNode->vNodeKeys.size())
		{
			_qRotation = _pAnimatedNode->vNodeKeys[iIndex].boneTransform.m_quaternion.ToGLM();
			return;
		}

		// Check if there is at least 2 frames
		if (iIndex == -1 || _pAnimatedNode->vNodeKeys.size() == 1)
		{
			_qRotation = _pAnimatedNode->vNodeKeys[0].boneTransform.m_quaternion.ToGLM();
			return;
		}

		// Interpolating frames
		float fDeltaTime = _pAnimatedNode->vNodeKeys[iNextIndex].fTimeStamp - _pAnimatedNode->vNodeKeys[iIndex].fTimeStamp;
		float fFactor = (_fAnimationTime - _pAnimatedNode->vNodeKeys[iIndex].fTimeStamp) / fDeltaTime;

		const glm::quat& qStart = _pAnimatedNode->vNodeKeys[iIndex].boneTransform.m_quaternion.ToGLM();
		const glm::quat& qEnd = _pAnimatedNode->vNodeKeys[iNextIndex].boneTransform.m_quaternion.ToGLM();

		InterpolateQuaternion(_qRotation, qStart, qEnd, fFactor);
		_qRotation = glm::normalize(_qRotation);
	}

	void SkinnedModel::CalcInterpolatedPosition(glm::vec3& _vPosition, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
	{
		int iIndex = FindAnimatedNodeIndex(_fAnimationTime, _pAnimatedNode);
		int iNextIndex = iIndex + 1;

		// Discard if next frame out of range
		if (iNextIndex == _pAnimatedNode->vNodeKeys.size())
		{
			_vPosition = _pAnimatedNode->vNodeKeys[iIndex].boneTransform.m_vLocalPosition.ToGLM();
			return;
		}

		// Check if there is at least 2 frames
		if (iIndex == -1 || _pAnimatedNode->vNodeKeys.size() == 1)
		{
			_vPosition = _pAnimatedNode->vNodeKeys[0].boneTransform.m_vLocalPosition.ToGLM();
			return;
		}

		// Interpolating frames
		float fDeltaTime = _pAnimatedNode->vNodeKeys[iNextIndex].fTimeStamp - _pAnimatedNode->vNodeKeys[iIndex].fTimeStamp;
		float fFactor = (_fAnimationTime - _pAnimatedNode->vNodeKeys[iIndex].fTimeStamp) / fDeltaTime;

		glm::vec3 vStart = _pAnimatedNode->vNodeKeys[iIndex].boneTransform.m_vLocalPosition.ToGLM();
		glm::vec3 vEnd = _pAnimatedNode->vNodeKeys[iNextIndex].boneTransform.m_vLocalPosition.ToGLM();

		glm::vec3 vDelta = vEnd - vStart;

		_vPosition = vStart + fFactor * vDelta;
	}

	void SkinnedModel::SetAnimation(const std::string& _strAnimation, bool _bLooping /*= true*/)
	{
		Shared<Animation> pAnimation = AssetManager::GetAsset<Animation>(hashID(_strAnimation.c_str()));
		if (pAnimation)
		{
			m_pCurrentAnimation = pAnimation;
			m_pCurrentAnimation->SetLooping(_bLooping);
		}
	}
}
