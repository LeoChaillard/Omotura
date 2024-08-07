#pragma once

#include "AssetInfo.h"

#include "../Common/Common.h"
#include "../Renderer/Mesh.h"

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Omotura
{
	struct MeshImportData
	{
		std::vector<Vector3> vVertices;
		std::vector<Vector3> vNormals;
		std::vector<Vector2> vUVs;
		std::vector<int> vIndices;
		std::vector<SubMeshDescriptor> vSubMeshes;
		std::vector<Material> vMaterials;
		std::vector<Joint> vJoints;
		std::map<NodeHandle, int> nodeBoneMap;
		std::vector<BoneInfo> vBonesInfos;
		glm::mat4 globalInverseTransform;
		std::vector<glm::ivec4> vBoneIDs;
		std::vector<glm::vec4> vWeights;
	};

	class MeshImporter
	{
	private:
		static int m_iNumVertices;
		static int m_iNumIndices;
	public:
		static Shared<Mesh> LoadMesh(const AssetInfo& _infos);
		static Shared<Mesh> LoadSkinnedMesh(const AssetInfo& _infos);

	private:
		// Non-skinned mesh
		static void InitFromScene(MeshImportData& _importData, const aiScene* _pScene, const std::string _strFileName, bool _bSkinned = false);
		static void CountVerticesAndIndices(MeshImportData& _importData, const aiScene* _pScene, bool _bSkinned = false);
		static void InitAllMeshes(MeshImportData& _importData, const aiScene* _pScene, bool _bSkinned = false);
		static void InitSingleMesh(MeshImportData& _importData, const aiMesh* _pAIMesh, int _iBaseVertex, bool _bSkinned = false);
		static void InitMaterials(MeshImportData& _importData, const aiScene* _pScene, const std::string& _strFileName);

		// Skeleton related
		static void RetrieveBones(MeshImportData& _importData, const aiMesh* _pAIMesh, int _iBaseVertex);
		static void RetrieveSkeletonJoints(MeshImportData& _importData, const aiNode* _pNode, int _iParentIndex);

		static void SetMeshData(Shared<Mesh> _pMesh, const MeshImportData& _importData);
	};
}