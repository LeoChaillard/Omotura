 #pragma once

#include "Scene.h"
#include "Player.h"

#include "../Renderer/SkinnedModel.h"
#include "../Renderer/Model.h"

namespace Omotura
{
	class Game
	{
	private:
		Scene m_scene;
		Shared<Player> m_pPlayer;

		float m_fDeltaTimeAccumulator;
		bool m_bIsLoaded;

	public:
		Game();
		~Game() = default;

		static Shared<Game> GetInstance()
		{
			static Shared<Game> spInstance = nullptr;
			if (spInstance == nullptr)
			{
				spInstance = CreateShared<Game>();
			}
			return spInstance;
		}

		void Update();
		void Create();
		bool IsLoaded();

		Shared<Camera> GetPlayerCamera();
		const Scene& GetScene();
		Shared<Player> GetPlayer();
	};
}