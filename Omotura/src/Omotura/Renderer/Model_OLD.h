#pragma once

#include "Material.h"
#include "Mesh.h"

#include "../Asset/Asset.h"
#include "../Core/Camera.h"
#include "../Common/Common.h"

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Omotura
{
	class Model_OLD : public Asset
	{
	private:
		GLuint m_EBO;
		GLuint m_VBO;
		GLuint m_VAO;

		enum BUFFER_TYPE
		{
			INDEX_BUFFER = 0,
			POS_VB = 1,
			NORMAL_VB = 2,
			TEXCOORD_VB = 3,
			WVP_MAT_VB = 4,
			WORLD_MAT_VB = 5,
			NUM_BUFFERS = 6
		};

		GLuint m_buffers[NUM_BUFFERS] = { 0 };

		std::vector<Mesh> m_vMeshes;
		std::vector<Material> m_vMaterials;

		// Temporary space for vertex attributes before we load them on the GPU
		std::vector<glm::vec3> m_vPositions;
		std::vector<glm::vec3> m_vNormals;
		std::vector<glm::vec2> m_vTextCoords;
		std::vector<GLuint> m_vIndices;

		glm::mat4 m_mModelMatrix = glm::mat4(1.0f);

	public:
		Model_OLD() = default;
		Model_OLD(const std::string & _strFilePath);
		~Model_OLD() = default;

		bool LoadMesh(const std::string& _strFilePath);

		void Render(Shader& _shader, Shared<Camera> _pCamera);
		void RenderSkybox(Shader& _shader, Shared<Camera> _pCamera);

		void SetModelMatrix(const glm::mat4& _mModelMatrix) { m_mModelMatrix = _mModelMatrix; }
		const glm::mat4& GetModelMatrix() { return m_mModelMatrix; } const

		static AssetType GetStaticType() { return AssetType::MODEL; }
		virtual AssetType GetType() const { return GetStaticType(); }

	private:
		bool InitFromScene(const aiScene* _pScene, const std::string _strFileName);
		void CountVerticesAndIndices(const aiScene* _pScene, unsigned int& _iNumVertices, unsigned int& _iNumIndices);
		void ReserveSpace(unsigned int _iNumVertices, unsigned int _iNumIndices);
		void InitAllMeshes(const aiScene* _pScene);
		void InitSingleMesh(const aiMesh* _pMesh);
		bool InitMaterials(const aiScene* _pScene, const std::string& _strFileName);
		void PopulateBuffers();
	};
}
