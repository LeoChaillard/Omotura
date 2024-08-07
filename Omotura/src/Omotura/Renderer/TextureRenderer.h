#pragma once

#include "SingleRenderer.h"

#include "Texture.h"

namespace Omotura
{
	class TextureRenderer : public SingleRenderer
	{
	private:
		Shared<Texture> m_pTexure;

	public:
		void Render(const RenderParams& _renderParams) override;

		void SetTexture(Shared<Texture> _pTexture);
	};
}