#pragma once

#include "Material.h"

#include "../Asset/Asset.h"
#include "../Utils/Math.hpp"
#include "../Core/GameObject.h"
#include "../Renderer/RendererCommon.h"

#include <vector>

namespace Omotura
{
	enum class MeshTopology
	{
		TRIANGLES,
		LINES,
		PATCHES
	};

	class Mesh : public GameObject, public Asset
	{
	private:
		// Buffers
		uint32_t m_VAO;
		uint32_t m_buffers[NUM_BUFFERS];

		// Mesh
		MeshTopology m_topology;
		std::vector<Vector3> m_vVertices;
		std::vector<Vector3> m_vNormals;
		std::vector<Vector2> m_vUVs;
		std::vector<int> m_vIndices;

		// SubMeshes
		std::vector<SubMeshDescriptor> m_vSubMeshes;
		std::vector<Material> m_vMaterials;

		// Skeleton related
		std::vector<Joint> m_vJoints;
		std::map<NodeHandle, int> m_nodeBoneMap;
		std::vector<BoneInfo> m_vBonesInfos;
		glm::mat4 m_globalInverseTransform;
		std::vector<glm::ivec4> m_vBoneIDs;
		std::vector<glm::vec4> m_vWeights;

		// Model
		glm::mat4 m_modelMatrix;

	public:
		Mesh();
		~Mesh() = default;

		// Upload data to gpu
		void PopulateBuffers();

		// Skeleton related
		void UpdateMatrixPalette(Shared<Animator> _pAnimator, float _fTimeInSeconds);

		// Model
		void SetModelMatrix(const glm::mat4& _mModelMatrix);
		const glm::mat4& GetModelMatrix() const;

		// Setters
		void SetTopology(MeshTopology _topology);
		void SetVertices(const std::vector<Vector3> _vVertices);
		void SetNormals(const std::vector<Vector3> _vNormals);
		void SetUVs(const std::vector<Vector2> _vUVs);
		void SetIndices(const std::vector<int> _vIndices);
		void SetSubMeshes(const std::vector<SubMeshDescriptor>& _vSubMeshes);
		void SetGlobalInverseTransform(const glm::mat4& _globalInverseTransform);
		void SetMaterials(const std::vector<Material>& _vMaterials);
		void SetBoneIDs(const std::vector<glm::ivec4>& _vBoneIDs);
		void SetBoneWeights(const std::vector<glm::vec4>& _vWeights);
		void SetBoneInfos(const std::vector<BoneInfo>& _vBoneInfos);
		void SetJoints(const std::vector<Joint>& _vJoints);
		void SetNodeBoneMap(const std::map<NodeHandle, int>& _nodeBoneMap);

		// Getters
		uint32_t GetVAO() const;
		GLenum GetTopology() const;
		const std::vector<Vector3>& GetVertices() const;
		const std::vector<Vector3>& GetNormals() const;
		const std::vector<Vector2>& GetUVs() const;
		const std::vector<int>& GetIndices() const;
		const std::vector<SubMeshDescriptor>& GetSubMeshes() const;
		const glm::mat4& GetGlobalInverseTransform() const;
		const std::vector<Material>& GetMaterials() const;
		const std::vector<glm::ivec4>& GetBoneIDs() const;
		const std::vector<glm::vec4>& GetBoneWeights() const;
		const std::vector<BoneInfo>& GetBoneInfos() const;
		const std::vector<Joint>& GetJoints() const;
		int FindAndGetBoneIndex(NodeHandle _handle) const;

		// Type
		static AssetType GetStaticType() { return AssetType::MESH; }
		virtual AssetType GetType() const { return GetStaticType(); }

	private:
		// Skeleton related
		void UpdateBoneTransformsFromBindPose(Shared<Animator> _pAnimator);
		void UpdateBoneTransformsFromAnimation(Shared<Animator> _pAnimator);

		// Animation related
		void CalcInterpolatedScaling(glm::vec3& _vScaling, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
		void CalcInterpolatedRotation(glm::quat& _qRotation, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
		void CalcInterpolatedPosition(glm::vec3& _vPosition, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
		int FindAnimatedNodeIndex(float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode);
	};
}
