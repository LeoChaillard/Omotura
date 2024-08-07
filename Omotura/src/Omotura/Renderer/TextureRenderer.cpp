#include "TextureRenderer.h"
#include "Mesh.h"

namespace Omotura
{
	void TextureRenderer::Render(const RenderParams& _renderParams)
	{
		if (m_pTexure == nullptr)
		{
			return;
		}

		Shared<Mesh> pQuad = AssetManager::GetAsset<Mesh>(hashID("Quad"));

		// Bind VAO
		glBindVertexArray(pQuad->GetVAO());

		// Bind Texture
		m_pTexure->Bind(TEXTURE_UNIT_DIFFUSE);

		// Camera
		Shared<Camera> pCamera = _renderParams.pCamera;
		_renderParams.pShader->Activate();
		_renderParams.pShader->SetMatrixFloat4("viewMatrix", pCamera->GetViewMatrix());
		_renderParams.pShader->SetMatrixFloat4("projectionMatrix", pCamera->GetPerspectiveMatrix());

		// Draw
		glDrawElements(pQuad->GetTopology(), pQuad->GetIndices().size(), GL_UNSIGNED_INT, 0);

		// Unbind
		m_pTexure->Unbind();

		// Unbind
		glBindVertexArray(0);
	}

	void TextureRenderer::SetTexture(Shared<Texture> _pTexture)
	{
		m_pTexure = _pTexture;
	}
}