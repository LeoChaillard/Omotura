#include "Model.h"

#include "../Asset/AssetImporter.h"
#include "../Asset/AssetManager.h"
#include "../API/OpenGLTexture.h"
#include "../Renderer/RendererCommon.h"

namespace Omotura
{
	Model::Model(const std::string& _strFilePath)
	{
		LoadModel(_strFilePath);
	}

	bool Model::LoadModel(const std::string& _strFilePath)
	{
		// Creating VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// Generating buffers for the vertices attributes
		glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);

		// Importing scene
		bool bSuccess = false;
		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(_strFilePath.c_str(), ASSIMP_LOAD_FLAGS);
		if (pScene)
		{
			bSuccess = InitFromScene(pScene, _strFilePath);
		}
		else
		{
			printf("Error parsing '%s': '%s'\n", _strFilePath.c_str(), importer.GetErrorString());
		}

		importer.FreeScene();

		// Unbinding VAO
		glBindVertexArray(0);
		
		return bSuccess;
	}

	bool Model::InitFromScene(const aiScene* _pScene, const std::string _strFileName)
	{
		m_vMeshes.resize(_pScene->mNumMeshes);
		m_vMaterials.resize(_pScene->mNumMaterials);

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

	void Model::CountVerticesAndIndices(const aiScene* _pScene, unsigned int& _iNumVertices, unsigned int& _iNumIndices)
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

	void Model::ReserveSpace(unsigned int _iNumVertices, unsigned int _iNumIndices)
	{
		m_vPositions.reserve(_iNumVertices);
		m_vNormals.reserve(_iNumVertices);
		m_vTextCoords.reserve(_iNumVertices);
		m_vIndices.reserve(_iNumIndices);
	}

	void Model::InitAllMeshes(const aiScene* _pScene)
	{
		int iSize = m_vMeshes.size();
		for (int i = 0; i < iSize; i++)
		{
			const aiMesh* paiMesh = _pScene->mMeshes[i];
			InitSingleMesh(paiMesh);
		}
	}

	void Model::InitSingleMesh(const aiMesh* _pMesh)
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

	bool Model::InitMaterials(const aiScene* _pScene, const std::string& _strFileName)
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

	void Model::PopulateBuffers()
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
	}

	GLuint Model::GetVAO()
	{
		return m_VAO;
	}

	const std::vector<Mesh>& Model::GetMeshes() const
	{
		return m_vMeshes;
	}

	const std::vector<Material>& Model::GetMaterials() const
	{
		return m_vMaterials;
	}
}
