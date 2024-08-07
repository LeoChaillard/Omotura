#pragma once

#include "SingleRenderer.h"
#include "Mesh.h"

namespace Omotura
{
	class MeshRenderer : public SingleRenderer
	{
	private:
		Shared<Mesh> m_pMesh;
		Material m_defaultMaterial;

	public:
		void Render(const RenderParams& _renderParams) override;

		void SetMesh(Shared<Mesh> _pMesh);
		void SetDefaultMaterial(const Material& _material) { m_defaultMaterial = _material; }
	};
}