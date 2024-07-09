#include "Model_OLD.h"

#include "../Asset/AssetImporter.h"
#include "../Asset/AssetManager.h"
#include "../API/OpenGLTexture.h"
#include "../Renderer/RendererCommon.h"

namespace Omotura
{
	Model_OLD::Model_OLD(const std::string& _strFilePath)
	{
		LoadMesh(_strFilePath);
	}

	bool Model_OLD::LoadMesh(const std::string& _strFilePath)
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

	bool Model_OLD::InitFromScene(const aiScene* _pScene, const std::string _strFileName)
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

	void Model_OLD::CountVerticesAndIndices(const aiScene* _pScene, unsigned int& _iNumVertices, unsigned int& _iNumIndices)
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

	void Model_OLD::ReserveSpace(unsigned int _iNumVertices, unsigned int _iNumIndices)
	{
		m_vPositions.reserve(_iNumVertices);
		m_vNormals.reserve(_iNumVertices);
		m_vTextCoords.reserve(_iNumVertices);
		m_vIndices.reserve(_iNumIndices);
	}

	void Model_OLD::InitAllMeshes(const aiScene* _pScene)
	{
		int iSize = m_vMeshes.size();
		for (int i = 0; i < iSize; i++)
		{
			const aiMesh* paiMesh = _pScene->mMeshes[i];
			InitSingleMesh(paiMesh);
		}
	}

	void Model_OLD::InitSingleMesh(const aiMesh* _pMesh)
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

	bool Model_OLD::InitMaterials(const aiScene* _pScene, const std::string& _strFileName)
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

	void Model_OLD::PopulateBuffers()
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

	void Model_OLD::Render(Shader& _shader, Shared<Camera> _pCamera)
	{
		// Bind
		_shader.Activate();
		glBindVertexArray(m_VAO);

		// Draw
		for (unsigned int i = 0; i < m_vMeshes.size(); i++)
		{
			unsigned int iMaterialIndex = m_vMeshes[i].iMaterialIndex;

			Shared<Texture> pDiffuse, pNormal, pRMA;
			if (iMaterialIndex != INVALID_MATERIAL)
			{
				assert(iMaterialIndex < m_vMaterials.size());

				pDiffuse = AssetManager::GetAsset<Texture>(m_vMaterials[iMaterialIndex].diffuseHandle);
				if (pDiffuse)
				{
					_shader.Activate();
					_shader.SetInt("material.diffuse0", TEXTURE_UNIT_DIFFUSE);
					_shader.SetFloat("material.shininess", 256.0f);
					pDiffuse->Bind(TEXTURE_UNIT_DIFFUSE);

					pNormal = AssetManager::GetAsset<Texture>(m_vMaterials[iMaterialIndex].normalHandle);
					if (pNormal)
					{
						_shader.Activate();
						_shader.SetInt("material.normal0", TEXTURE_UNIT_NORMAL);
						pNormal->Bind(TEXTURE_UNIT_NORMAL);
					}

					pRMA = AssetManager::GetAsset<Texture>(m_vMaterials[iMaterialIndex].rmaHandle);
					if (pRMA)
					{
						_shader.Activate();
						_shader.SetInt("material.specular0", TEXTURE_UNIT_RMA);
						pRMA->Bind(TEXTURE_UNIT_RMA);
					}
				}
				else
				{
					pDiffuse = AssetManager::GetAsset<Texture>(hashID("NoTexture"));
					_shader.Activate();
					_shader.SetInt("material.diffuse0", TEXTURE_UNIT_DIFFUSE);
					_shader.SetFloat("material.shininess", 256.0f);
					pDiffuse->Bind(TEXTURE_UNIT_DIFFUSE);
				}
			}

			// Camera
			_shader.SetFloat3("camPos", _pCamera->m_vPosition.x, _pCamera->m_vPosition.y, _pCamera->m_vPosition.z);
			_pCamera->UploadMatrix(_shader, "camMatrix");

			// Draw
			glDrawElementsBaseVertex(GL_TRIANGLES,
				m_vMeshes[i].iNumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(GLuint) * m_vMeshes[i].iBaseIndex),
				m_vMeshes[i].iBaseVertex);

			// Unbind textures
			if (pDiffuse) pDiffuse->Unbind();
			if (pNormal) pNormal->Unbind();
			if (pRMA) pRMA->Unbind();
		}

		glBindVertexArray(0);
	}

	void Model_OLD::RenderSkybox(Shader& _shader, Shared<Camera> _pCamera)
	{
		// Camera
		glDepthFunc(GL_LEQUAL);

		_shader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(_pCamera->m_vPosition, _pCamera->m_vPosition + _pCamera->m_vFront, _pCamera->m_vUp)));
		projection = glm::perspective(glm::radians(45.0f), (float)1920 / 1080, 0.1f, 100.0f);
		_shader.SetMatrixFloat4("view", view);
		_shader.SetMatrixFloat4("projection", projection);

		// Bind
		glBindVertexArray(m_VAO);

		_shader.Activate();
		_shader.SetInt("skybox", GL_TEXTURE0);
		AssetManager::GetAsset<Texture>(hashID("NightSky"))->Bind(GL_TEXTURE0);

		// Draw
		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_vMeshes[0].iNumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(GLuint) * m_vMeshes[0].iBaseIndex),
			m_vMeshes[0].iBaseVertex);

		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
}
