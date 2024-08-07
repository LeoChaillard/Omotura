#include "Mesh.h"

namespace Omotura
{
	Mesh::Mesh()
	: m_topology(MeshTopology::TRIANGLES), m_vVertices(), m_vNormals(), m_vUVs(), m_vIndices(), m_vSubMeshes(), m_vMaterials(), m_vJoints(), m_nodeBoneMap(), m_vBonesInfos(), m_globalInverseTransform(), m_vWeights(), m_vBoneIDs(), m_modelMatrix(glm::mat4(1.0f))
	{
	}

	void Mesh::UpdateBoneTransformsFromBindPose(Shared<Animator> _pAnimator)
	{
		int iJoints = (int)m_vJoints.size();
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

	void Mesh::UpdateBoneTransformsFromAnimation(Shared<Animator> _pAnimator)
	{
		float fAnimationTime = _pAnimator->GetAnimationTime();
		int iJoints = (int)m_vJoints.size();
		for (int i = 0; i < iJoints; i++)
		{
			// Get the node and its transform matrix
			NodeHandle nodeHandle = m_vJoints[i].handle;
			glm::mat4 nodeTransform = m_vJoints[i].nodeTransform;

			// Make animation transitions
			Shared<AnimatedNode> pAnimatedNode = _pAnimator->FindAnimatedNode(nodeHandle);
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

	void Mesh::CalcInterpolatedScaling(glm::vec3& _vScaling, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
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

	void Mesh::CalcInterpolatedRotation(glm::quat& _qRotation, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
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

	void Mesh::CalcInterpolatedPosition(glm::vec3& _vPosition, float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
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

	int Mesh::FindAnimatedNodeIndex(float _fAnimationTime, const Shared<AnimatedNode> _pAnimatedNode)
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

	void Mesh::UpdateMatrixPalette(Shared<Animator> _pAnimator, float _fTimeInSeconds)
	{
		if (this->m_strName != _pAnimator->GetAnimatedMesh())
		{
			UpdateBoneTransformsFromBindPose(_pAnimator);
			return;
		}

		_pAnimator->UpdateAnimation(_fTimeInSeconds);
		UpdateBoneTransformsFromAnimation(_pAnimator);
	}

	void Mesh::PopulateBuffers()
	{
		// Creating VAO		
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// Generating buffers for the vertices attributes
		glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);

		// Bind and setup buffers
		if (!m_vIndices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vIndices[0]) * m_vIndices.size(), m_vIndices.data(), GL_STATIC_DRAW);
		}

		if (!m_vVertices.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vVertices[0]) * m_vVertices.size(), m_vVertices.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(ATTRIBUTE_POSITION_LOCATION);
			glVertexAttribPointer(ATTRIBUTE_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (!m_vNormals.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vNormals[0]) * m_vNormals.size(), m_vNormals.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(ATTRIBUTE_NORMAL_LOCATION);
			glVertexAttribPointer(ATTRIBUTE_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (!m_vUVs.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vUVs[0]) * m_vUVs.size(), m_vUVs.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(ATTRIBUTE_TEX_COORD_LOCATION);
			glVertexAttribPointer(ATTRIBUTE_TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (!m_vBoneIDs.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_ID_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vBoneIDs[0]) * m_vBoneIDs.size(), m_vBoneIDs.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(ATTRIBUTE_BONE_ID_LOCATION);
			glVertexAttribIPointer(ATTRIBUTE_BONE_ID_LOCATION, 4, GL_INT, 0, 0);
		}

		if (!m_vWeights.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_WEIGHT_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vWeights[0]) * m_vWeights.size(), m_vWeights.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(ATTRIBUTE_BONE_WEIGHT_LOCATION);
			glVertexAttribPointer(ATTRIBUTE_BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}

	void Mesh::SetModelMatrix(const glm::mat4& _mModelMatrix) 
	{
		m_modelMatrix = _mModelMatrix; 
	}

	const glm::mat4& Mesh::GetModelMatrix() const
	{ 
		return m_modelMatrix; 
	} 

	void Mesh::SetTopology(MeshTopology _topology)
	{
		m_topology = _topology;
	}

	void Mesh::SetVertices(const std::vector<Vector3> _vVertices)
	{
		m_vVertices = _vVertices;
	}

	void Mesh::SetNormals(const std::vector<Vector3> _vNormals)
	{
		m_vNormals = _vNormals;
	}

	void Mesh::SetUVs(const std::vector<Vector2> _vUVs)
	{
		m_vUVs = _vUVs;
	}

	void Mesh::SetIndices(const std::vector<int> _vIndices)
	{
		m_vIndices = _vIndices;
	}

	void Mesh::SetSubMeshes(const std::vector<SubMeshDescriptor>& _vSubMeshes)
	{
		m_vSubMeshes = _vSubMeshes;
	}

	void Mesh::SetGlobalInverseTransform(const glm::mat4& _globalInverseTransform)
	{
		m_globalInverseTransform = _globalInverseTransform;
	}

	void Mesh::SetMaterials(const std::vector<Material>& _vMaterials)
	{
		m_vMaterials = _vMaterials;
	}

	void Mesh::SetBoneIDs(const std::vector<glm::ivec4>& _vBoneIDs)
	{
		m_vBoneIDs = _vBoneIDs;
	}

	void Mesh::SetBoneWeights(const std::vector<glm::vec4>& _vWeights)
	{
		m_vWeights = _vWeights;
	}

	void Mesh::SetBoneInfos(const std::vector<BoneInfo>& _vBoneInfos)
	{
		m_vBonesInfos = _vBoneInfos;
	}

	void Mesh::SetJoints(const std::vector<Joint>& _vJoints)
	{
		m_vJoints = _vJoints;
	}

	void Mesh::SetNodeBoneMap(const std::map<NodeHandle, int>& _nodeBoneMap)
	{
		m_nodeBoneMap = _nodeBoneMap;
	}

	uint32_t Mesh::GetVAO() const
	{
		return m_VAO;
	}

	GLenum Mesh::GetTopology() const
	{
		switch (m_topology)
		{
			case MeshTopology::TRIANGLES: return GL_TRIANGLES;
			case MeshTopology::LINES: return GL_LINES;
			case MeshTopology::PATCHES: return GL_PATCHES;
		}
		return GL_TRIANGLES;
	}

	const std::vector<Vector3>& Mesh::GetVertices() const
	{
		return m_vVertices;
	}

	const std::vector<Vector3>& Mesh::GetNormals() const
	{
		return m_vNormals;
	}

	const std::vector<Vector2>& Mesh::GetUVs() const
	{
		return m_vUVs;
	}

	const std::vector<int>& Mesh::GetIndices() const
	{
		return m_vIndices;
	}

	const std::vector<SubMeshDescriptor>& Mesh::GetSubMeshes() const
	{
		return m_vSubMeshes;
	}

	const glm::mat4& Mesh::GetGlobalInverseTransform() const
	{
		return m_globalInverseTransform;
	}

	const std::vector<Material>& Mesh::GetMaterials() const
	{
		return m_vMaterials;
	}

	const std::vector<glm::ivec4>& Mesh::GetBoneIDs() const
	{
		return m_vBoneIDs;
	}

	const std::vector<glm::vec4>& Mesh::GetBoneWeights() const
	{
		return m_vWeights;
	}

	const std::vector<BoneInfo>& Mesh::GetBoneInfos() const
	{
		return m_vBonesInfos;
	}

	const std::vector<Joint>& Mesh::GetJoints() const
	{
		return m_vJoints;
	}

	int Mesh::FindAndGetBoneIndex(NodeHandle _handle) const
	{
		if (m_nodeBoneMap.find(_handle) == m_nodeBoneMap.end())
		{
			return -1;
		}
		return m_nodeBoneMap.at(_handle);
	}
}