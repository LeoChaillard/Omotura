#pragma once

#include "../Core/UUID.h"
#include "../Core/Camera.h"

#include <glm/glm.hpp>

namespace Omotura
{
	enum class DrawMode
	{
		ARRAYS,
		ELEMENTS,
		ELEMENTS_BASE_VERTEX
	};

	struct RenderParams
	{
		Shared<Camera> pCamera;
		Shared<Shader> pShader;
		DrawMode drawMode = DrawMode::ELEMENTS_BASE_VERTEX;
		bool bBindMaterial = true;
	};

	struct SubMeshDescriptor
	{
		uint32_t iNumIndices;
		uint32_t iNumVertices;
		uint32_t iBaseVertex;
		uint32_t iBaseIndex;
		uint32_t iMaterialIndex;

		SubMeshDescriptor() = default;
		SubMeshDescriptor(uint32_t _iNumIndices, uint32_t _iNumVertices, uint32_t _iBaseVertex, uint32_t _iBaseIndex, uint32_t _iMaterialIndex) : iNumIndices(_iNumIndices), iNumVertices(_iNumVertices), iBaseVertex(_iBaseVertex), iBaseIndex(_iBaseIndex), iMaterialIndex(_iMaterialIndex) {}
	};

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

	enum class API
	{
		OPENGL
	};

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

	#define ATTRIBUTE_POSITION_LOCATION 0
	#define ATTRIBUTE_NORMAL_LOCATION 1
	#define ATTRIBUTE_TEX_COORD_LOCATION 2
	#define ATTRIBUTE_BONE_ID_LOCATION 3
	#define ATTRIBUTE_BONE_WEIGHT_LOCATION 4

	#define TEXTURE_UNIT_DIFFUSE GL_TEXTURE0
	#define TEXTURE_UNIT_NORMAL GL_TEXTURE1
	#define TEXTURE_UNIT_RMA GL_TEXTURE2
}