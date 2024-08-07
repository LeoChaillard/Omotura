#pragma once

#include "MeshImporter.h"

namespace Omotura
{
	int MeshImporter::m_iNumVertices;
	int MeshImporter::m_iNumIndices;

	/*******************NON-SKINNED MESH************************/
	Shared<Mesh> MeshImporter::LoadMesh(const AssetInfo& _infos)
	{
		// Creating mesh
		Shared<Mesh> pMesh = CreateShared<Mesh>();

		// Importing scene
		Assimp::Importer importer;

		std::string strPath = std::get<std::string>(_infos.strFilePath);
		const aiScene* pScene = importer.ReadFile(strPath.c_str(), ASSIMP_LOAD_FLAGS);
		if (pScene)
		{
			MeshImportData importData;
			InitFromScene(importData, pScene, strPath);
			SetMeshData(pMesh, importData);
			pMesh->PopulateBuffers();
		}
		else
		{
			printf("Error parsing '%s': '%s'\n", strPath.c_str(), importer.GetErrorString());
		}

		importer.FreeScene();

		return pMesh;
	}

	void MeshImporter::InitFromScene(MeshImportData& _importData, const aiScene* _pScene, const std::string _strFileName, bool _bSkinned /*= false*/)
	{
		m_iNumVertices = 0;
		m_iNumIndices = 0;

		CountVerticesAndIndices(_importData, _pScene, _bSkinned);
		InitAllMeshes(_importData, _pScene, _bSkinned);
		InitMaterials(_importData, _pScene, _strFileName);
	}

	void MeshImporter::CountVerticesAndIndices(MeshImportData& _importData, const aiScene* _pScene, bool _bSkinned /*= false*/)
	{
		// Resizing meshes and materials
		_importData.vSubMeshes.resize(_pScene->mNumMeshes);
		_importData.vMaterials.resize(_pScene->mNumMaterials);

		int iSize = _pScene->mNumMeshes;
		for (unsigned int i = 0; i < iSize; i++)
		{
			_importData.vSubMeshes[i].iMaterialIndex = _pScene->mMeshes[i]->mMaterialIndex;
			_importData.vSubMeshes[i].iNumVertices = _pScene->mMeshes[i]->mNumVertices;
			_importData.vSubMeshes[i].iNumIndices = _pScene->mMeshes[i]->mNumFaces * 3;
			_importData.vSubMeshes[i].iBaseVertex = m_iNumVertices;
			_importData.vSubMeshes[i].iBaseIndex = m_iNumIndices;

			m_iNumVertices += _importData.vSubMeshes[i].iNumVertices;
			m_iNumIndices += _importData.vSubMeshes[i].iNumIndices;
		}

		// Reserving or resizing attributes
		_importData.vVertices.reserve(m_iNumVertices);
		_importData.vNormals.reserve(m_iNumVertices);
		_importData.vUVs.reserve(m_iNumVertices);
		_importData.vIndices.reserve(m_iNumIndices);

		if (_bSkinned)
		{
			_importData.vBoneIDs.resize(m_iNumVertices);
			_importData.vWeights.resize(m_iNumVertices);
		}
	}

	void MeshImporter::InitAllMeshes(MeshImportData& _importData, const aiScene* _pScene, bool _bSkinned /*= false*/)
	{
		int iSize = _importData.vSubMeshes.size();
		for (int i = 0; i < iSize; i++)
		{
			const aiMesh* paiMesh = _pScene->mMeshes[i];
			InitSingleMesh(_importData, paiMesh, _importData.vSubMeshes[i].iBaseVertex, _bSkinned);
		}

		if (_bSkinned)
		{
			// Get skeleton joints/nodes
			RetrieveSkeletonJoints(_importData, _pScene->mRootNode, -1);
			_importData.globalInverseTransform = glm::inverse(AiMatrix4x4ToGlm(_pScene->mRootNode->mTransformation));
		}
	}

	void MeshImporter::InitSingleMesh(MeshImportData& _importData, const aiMesh* _pAIMesh, int _iBaseVertex, bool _bSkinned /*= false*/)
	{
		// Populate the vertex attribute vectors
		int iVertices = _pAIMesh->mNumVertices;
		for (int i = 0; i < iVertices; i++)
		{
			const aiVector3D& pos = _pAIMesh->mVertices[i];
			const aiVector3D& normal = _pAIMesh->mNormals[i];
			const aiVector3D& textCoord = _pAIMesh->HasTextureCoords(0) ? _pAIMesh->mTextureCoords[0][i] : aiVector3D(0.0f);

			_importData.vVertices.push_back(Vector3(pos.x, pos.y, pos.z));
			_importData.vNormals.push_back(Vector3(normal.x, normal.y, normal.z));
			_importData.vUVs.push_back(Vector2(textCoord.x, textCoord.y));
		}

		if (_bSkinned)
		{
			// Retrieve bones
			RetrieveBones(_importData, _pAIMesh, _iBaseVertex);
		}

		// Populate the index buffer
		int iFaces = _pAIMesh->mNumFaces;
		for (int i = 0; i < iFaces; i++)
		{
			const aiFace& face = _pAIMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			_importData.vIndices.push_back(face.mIndices[0]);
			_importData.vIndices.push_back(face.mIndices[1]);
			_importData.vIndices.push_back(face.mIndices[2]);
		}
	}

	void MeshImporter::InitMaterials(MeshImportData& _importData, const aiScene* _pScene, const std::string& _strFileName)
	{
		// Initialize the materials
		int iMaterials = _pScene->mNumMaterials;
		for (int i = 0; i < iMaterials; i++)
		{
			const aiMaterial* pMaterial = _pScene->mMaterials[i];

			aiString materialName = pMaterial->GetName();
			std::string strName(materialName.C_Str());

			_importData.vMaterials[i].diffuseHandle = hashID((strName + std::string("_ALB")).c_str());
			_importData.vMaterials[i].normalHandle = hashID((strName + std::string("_NRM")).c_str());
			_importData.vMaterials[i].rmaHandle = hashID((strName + std::string("_RMA")).c_str());
		}
	}

	/*******************SKINNED MESH************************/
	Shared<Mesh> MeshImporter::LoadSkinnedMesh(const AssetInfo& _infos)
	{
		// Creating mesh
		Shared<Mesh> pMesh = CreateShared<Mesh>();

		// Importing scene
		Assimp::Importer importer;

		std::string strPath = std::get<std::string>(_infos.strFilePath);
		const aiScene* pScene = importer.ReadFile(strPath.c_str(), ASSIMP_LOAD_FLAGS);
		if (pScene)
		{
			MeshImportData importData;
			InitFromScene(importData, pScene, strPath, true);
			SetMeshData(pMesh, importData);
			pMesh->PopulateBuffers();
		}
		else
		{
			printf("Error parsing '%s': '%s'\n", strPath.c_str(), importer.GetErrorString());
		}

		importer.FreeScene();

		return pMesh;
	}

	void MeshImporter::RetrieveBones(MeshImportData& _importData, const aiMesh* _pAIMesh, int _iBaseVertex)
	{
		// Load infos
		int iSize = _pAIMesh->mNumBones;
		for (int i = 0; i < iSize; i++)
		{
			aiBone* pBone = _pAIMesh->mBones[i];
			std::string strBoneName = pBone->mName.C_Str();
			BoneHandle boneHandle = hashID(strBoneName.c_str());

			// Create bone if doesn't exist (may be shared between meshes)
			if (_importData.nodeBoneMap.find(boneHandle) == _importData.nodeBoneMap.end())
			{
				BoneInfo boneInfo;
				boneInfo.handle = boneHandle;
				boneInfo.offsetMatrix = AiMatrix4x4ToGlm(_pAIMesh->mBones[i]->mOffsetMatrix);
				_importData.nodeBoneMap[boneHandle] = _importData.vBonesInfos.size();
				_importData.vBonesInfos.push_back(boneInfo);
			}

			// Retrieve weights and bone IDs
			int iWeights = pBone->mNumWeights;
			for (int j = 0; j < iWeights; j++)
			{
				int iBoneIndex = _importData.nodeBoneMap[boneHandle];
				const aiVertexWeight& vertexWeight = pBone->mWeights[j];
				int iGlobalVertexID = _iBaseVertex + vertexWeight.mVertexId;
				if (_importData.vWeights[iGlobalVertexID].x == 0)
				{
					_importData.vBoneIDs[iGlobalVertexID].x = iBoneIndex;
					_importData.vWeights[iGlobalVertexID].x = vertexWeight.mWeight;
				}
				else if (_importData.vWeights[iGlobalVertexID].y == 0)
				{
					_importData.vBoneIDs[iGlobalVertexID].y = iBoneIndex;
					_importData.vWeights[iGlobalVertexID].y = vertexWeight.mWeight;
				}
				else if (_importData.vWeights[iGlobalVertexID].z == 0)
				{
					_importData.vBoneIDs[iGlobalVertexID].z = iBoneIndex;
					_importData.vWeights[iGlobalVertexID].z = vertexWeight.mWeight;
				}
				else if (_importData.vWeights[iGlobalVertexID].w == 0)
				{
					_importData.vBoneIDs[iGlobalVertexID].w = iBoneIndex;
					_importData.vWeights[iGlobalVertexID].w = vertexWeight.mWeight;
				}
			}
		}
	}

	void MeshImporter::RetrieveSkeletonJoints(MeshImportData& _importData, const aiNode* _pNode, int _iParentIndex)
	{
		Joint joint;
		joint.handle = hashID(_pNode->mName.data);
		joint.parentIndex = _iParentIndex; // Need to keep track of parent as it is recursive
		joint.nodeTransform = AiMatrix4x4ToGlm(_pNode->mTransformation);

		_iParentIndex = _importData.vJoints.size();
		_importData.vJoints.push_back(joint);

		int iChildren = _pNode->mNumChildren;
		for (int i = 0; i < iChildren; i++)
		{
			RetrieveSkeletonJoints(_importData, _pNode->mChildren[i], _iParentIndex);
		}
	}

	void MeshImporter::SetMeshData(Shared<Mesh> _pMesh, const MeshImportData& _importData)
	{
		_pMesh->SetVertices(_importData.vVertices);
		_pMesh->SetNormals(_importData.vNormals);
		_pMesh->SetUVs(_importData.vUVs);
		_pMesh->SetIndices(_importData.vIndices);
		_pMesh->SetSubMeshes(_importData.vSubMeshes);
		_pMesh->SetMaterials(_importData.vMaterials);
		_pMesh->SetJoints(_importData.vJoints);
		_pMesh->SetNodeBoneMap(_importData.nodeBoneMap);
		_pMesh->SetBoneInfos(_importData.vBonesInfos);
		_pMesh->SetGlobalInverseTransform(_importData.globalInverseTransform);
		_pMesh->SetBoneIDs(_importData.vBoneIDs);
		_pMesh->SetBoneWeights(_importData.vWeights);
	}
}