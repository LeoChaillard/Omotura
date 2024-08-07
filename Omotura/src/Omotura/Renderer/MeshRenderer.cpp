#include "MeshRenderer.h"
#include "RendererCommon.h"
#include "Texture.h"

#include "../Asset/AssetManager.h"

namespace Omotura
{
	void MeshRenderer::Render(const RenderParams& _renderParams)
	{
		if (m_pMesh == nullptr)
		{
			return;
		}

		// Bind VAO
		glBindVertexArray(m_pMesh->GetVAO());

		std::vector<Material> vMaterials = m_pMesh->GetMaterials();
		std::vector<SubMeshDescriptor> vSubMeshes = m_pMesh->GetSubMeshes();
		int iMeshes = (int)vSubMeshes.size();
		for (int j = 0; j < iMeshes; j++)
		{
			// Bind Material
			if (_renderParams.bBindMaterial)
			{
				if (vMaterials.empty())
				{
					m_defaultMaterial.Bind();
				}
				else
				{
					assert(vSubMeshes[j].iMaterialIndex < vMaterials.size());
					vMaterials[vSubMeshes[j].iMaterialIndex].Bind();
				}
			}

			// Camera
			Shared<Camera> pCamera = _renderParams.pCamera;
			_renderParams.pShader->Activate();
			_renderParams.pShader->SetMatrixFloat4("modelMatrix", m_pMesh->GetModelMatrix());
			_renderParams.pShader->SetMatrixFloat4("viewMatrix", pCamera->GetViewMatrix());
			_renderParams.pShader->SetMatrixFloat4("projectionMatrix", pCamera->GetPerspectiveMatrix());

			// Draw
			if (_renderParams.drawMode == DrawMode::ARRAYS)
			{
				glDrawArrays(m_pMesh->GetTopology(), 0, vSubMeshes[j].iNumVertices);
			}
			else if (_renderParams.drawMode == DrawMode::ELEMENTS)
			{
				glDrawElements(GL_TRIANGLES, vSubMeshes[j].iNumIndices, GL_UNSIGNED_INT, 0);
			}
			else if (_renderParams.drawMode == DrawMode::ELEMENTS_BASE_VERTEX)
			{
				glDrawElementsBaseVertex(m_pMesh->GetTopology(),
					vSubMeshes[j].iNumIndices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(GLuint) * vSubMeshes[j].iBaseIndex),
					vSubMeshes[j].iBaseVertex);
			}

			// Unbind
			if (_renderParams.bBindMaterial)
			{
				if (vMaterials.empty())
				{
					m_defaultMaterial.Unbind();
				}
				else
				{
					vMaterials[vSubMeshes[j].iMaterialIndex].Unbind();
				}
			}
		}		

		// Unbind
		glBindVertexArray(0);
	}

	void MeshRenderer::SetMesh(Shared<Mesh> _pMesh)
	{
		m_pMesh = _pMesh;
	}
}