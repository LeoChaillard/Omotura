#include "Game.h"
#include "Time.h"
#include "AnimatorManager.h"
#include "PlayerInput.h"

#include "../BackEnd/BackEnd.h"
#include "../Utils/Utils.hpp"

#include <string>

namespace Omotura
{
	Game g_gameInstance;

	Game::Game()
		: m_scene(),
		m_pPlayer(nullptr),
		m_fDeltaTimeAccumulator()
	{
		m_pPlayer = CreateShared<Player>();
	}

	void Game::Init()
	{
		m_scene.Init();
		m_pPlayer->Init();
	}

	void Game::Update()
	{
		// Update Time
		Time::Update();
		float fDeltaTime = Time::GetDeltaTime();
		
		// Update fixed delta
		m_fDeltaTimeAccumulator += fDeltaTime;
		if (m_fDeltaTimeAccumulator >= Time::GetFixedDeltaTime())
		{
			// FPS
			std::string strFPS = std::to_string(1.0f / m_fDeltaTimeAccumulator);
			std::string strTitle = Utils::GetEngineName() + " - " + strFPS + "FPS";
			glfwSetWindowTitle(BackEnd::GetGLFWwindow(), strTitle.c_str());			

			// Update animators
			AnimatorManager::FixedUpdate();
			
			// Player
			m_pPlayer->FixedUpdate();

			// Step physics
			m_fDeltaTimeAccumulator = 0.0f;
		}
		
		// Update Animators
		AnimatorManager::Update();

		// Update Scene / Update scene game objects (use buckets? pre/post/final animation?)
		m_scene.Update(fDeltaTime);

		// Update Player
		PlayerInput::Update();
		m_pPlayer->Update(fDeltaTime);
	}

	Shared<Camera> Game::GetPlayerCamera()
	{
		return m_pPlayer->GetCamera();
	}

	const Scene& Game::GetScene() const
	{
		return m_scene;
	}

	Shared<Player> Game::GetPlayer()
	{
		return m_pPlayer;
	}
}
