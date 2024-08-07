#pragma once

#include "Terrain/Terrain.h"

#include "../Renderer/Mesh.h"

#include <vector>

namespace Omotura
{
	class Scene
	{
	private:
		// Scene Objects
		std::vector<Shared<Mesh>> m_vpMeshes;
		Shared<Terrain> m_pTerrain;

	public:
		Scene();
		~Scene() = default;

		void Init();
		void Update(float _fDeltaTime);

		const std::vector<Shared<Mesh>>& GetMeshes() const;
		Shared<Terrain> GetTerrain() const;
	};
}