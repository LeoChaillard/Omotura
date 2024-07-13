#pragma once

#include "Material.h"
#include "Mesh.h"

#include "../Asset/Asset.h"
#include "../Core/Camera.h"
#include "../Core/Animation.h"
#include "../Common/Common.h"

#include <string>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Omotura
{
	using NodeHandle = UUID;
	struct Joint
	{
		NodeHandle handle;
		int parentIndex;
		glm::mat4 nodeTransform;
		glm::mat4 globalTransform;
	};

	using BoneHandle = UUID;
	struct BoneInfo
	{
		BoneHandle handle;
		glm::mat4 offsetMatrix;
		glm::mat4 finalTransform;
		glm::mat4 modelSpaceTransform;
	};

	class SkinnedModel : public Asset
	{
	private:
		const aiScene* m_pScene = nullptr;
		Assimp::Importer m_importer;

		GLuint m_EBO;
		GLuint m_VBO;
		GLuint m_VAO;

		enum BUFFER_TYPE
		{
			INDEX_BUFFER = 0,
			POS_VB = 1,
			NORMAL_VB = 2,
			TEXCOORD_VB = 3,
			BONE_ID_VB = 4,
			BONE_WEIGHT_VB = 5,
			NUM_BUFFERS = 6
		};

		GLuint m_buffers[NUM_BUFFERS] = { 0 };

		// Model
		std::vector<Mesh> m_vMeshes;
		std::vector<Material> m_vMaterials;
		glm::mat4 m_mModelMatrix = glm::mat4(1.0f);

		// Temporary space for vertex attributes before we load them on the GPU
		std::vector<glm::vec3> m_vPositions;
		std::vector<glm::vec3> m_vNormals;
		std::vector<glm::vec2> m_vTextCoords;
		std::vector<GLuint> m_vIndices;

		// Sekelton related
		std::vector<Joint> m_vJoints;
		std::map<NodeHandle, int> m_nodeBoneMap;
		std::vector<BoneInfo> m_vBonesInfos;
		glm::mat4 m_globalInverseTransform;
		std::vector<glm::vec4> m_vWeights;
		std::vector<glm::ivec4> m_vBoneIDs;

		// Animation related
		Shared<Animation> m_pCurrentAnimation;
		float m_fCurrentAnimationTime = 0.0f;

	public:
		SkinnedModel() = default;
		SkinnedModel(const std::string& _strFilePath);
		~SkinnedModel();

		// Import related
		bool LoadSkinnedModel(const std::string& _strFilePath);

		// Skeleton related
		void UpdateBoneTransformsFromBindPose();
		void UpdateBoneTransformsFromAnimation();
		void GetBoneTransforms(float _fTimeInSeconds, std::vector<glm::mat4>& _transforms);

		// Animation related
		void SetAnimation(const std::string& _strAnimation, bool _bLooping = true);

		// Model
		GLuint GetVAO();
		const std::vector<Mesh>& GetMeshes() const;
		const std::vector<Material>& GetMaterials() const;

		void SetModelMatrix(const glm::mat4& _mModelMatrix) { m_mModelMatrix = _mModelMatrix; }
		const glm::mat4& GetModelMatrix() { return m_mModelMatrix; } const

		static AssetType GetStaticType() { return AssetType::SKINNEDMODEL; }
		virtual AssetType GetType() const { return GetStaticType(); }

	private:
		// Import related
		bool InitFromScene(const aiScene* _pScene, const std::string _strFileName);
		void CountVerticesAndIndices(const aiScene* _pScene, unsigned int& _iNumVertices, unsigned int& _iNumIndices);
		void ReserveSpace(unsigned int _iNumVertices, unsigned int _iNumIndices);
		void InitAllMeshes(const aiScene* _pScene);
		void InitSingleMesh(const aiMesh* _pMesh, int _iBaseVertex);
		bool InitMaterials(const aiScene* _pScene, const std::string& _strFileName);
		void PopulateBuffers();

		// Skeleton related
		void RetrieveBones(const aiMesh* _pMesh, int _iBaseVertex);
		void RetrieveSkeletonJoints(const aiNode* _pNode, int _iParentIndex);

		// Animated related
		Shared<AnimatedNode> FindAnimatedNode(Shared<Animation> _pAnimation, BoneHandle _handle);
		int FindAnimatedNodeIndex(float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
		float GetAnimationTime(float _fCurrentAnimationTime, Shared<Animation> _pCurrentAnimation);
		void CalcInterpolatedScaling(glm::vec3& _vScaling, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
		void CalcInterpolatedRotation(glm::quat& _qRotation, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
		void CalcInterpolatedPosition(glm::vec3& _vPosition, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
		void UpdateAnimation(float _fDeltaTime);
	};
}
