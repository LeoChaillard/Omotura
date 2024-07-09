#pragma once

#include "FlyCamera.h"
#include "FPSCamera.h"
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
		Player m_player;

		float m_fDeltaTimeAccumulator;
		float m_fFixedDeltaTime;
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

		void Update(float _fDeltaTime);
		void Create();
		bool IsLoaded();

		Shared<Camera> GetPlayerCamera();
		const Scene& GetScene();
		const Player& GetPlayer();
	};
}