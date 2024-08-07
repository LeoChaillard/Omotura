 #pragma once

#include "Scene.h"
#include "Player.h"

namespace Omotura
{
	class Game
	{
	private:
		Scene m_scene;
		Shared<Player> m_pPlayer;

		float m_fDeltaTimeAccumulator;

	public:
		Game();
		~Game() = default;

		void Init();
		void Update();

		Shared<Camera> GetPlayerCamera();
		const Scene& GetScene() const;
		Shared<Player> GetPlayer();
	};
}