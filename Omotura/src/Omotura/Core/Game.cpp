#include "Game.h"

#include "../BackEnd/BackEnd.h"
#include "../Utils/Utils.hpp"

#include <string>

namespace Omotura
{
	Game::Game()
		: m_scene(),
		m_player(),
		m_fDeltaTimeAccumulator(),
		m_fFixedDeltaTime(1.0f / 60.0f),
		m_bIsLoaded(false)
	{
	}

	void Game::Create()
	{
		m_bIsLoaded = true;
	}

	bool Game::IsLoaded()
	{
		return true;
	}

	void Game::Update(float _fDeltaTime)
	{
		// Update fixed delta
		m_fDeltaTimeAccumulator += _fDeltaTime;

		if (m_fDeltaTimeAccumulator >= m_fFixedDeltaTime)
		{
			// FPS
			std::string strFPS = std::to_string(1.0f / m_fDeltaTimeAccumulator);
			std::string strTitle = Utils::GetEngineName() + " - " + strFPS + "FPS";
			glfwSetWindowTitle(BackEnd::GetGLFWwindow(), strTitle.c_str());			

			// Player
			m_player.FixedUpdate(m_fFixedDeltaTime);

			// Step physics
			m_fDeltaTimeAccumulator = 0.0f;

		}

		// Update Player
		m_player.Update(_fDeltaTime);

		// Update Scene / Update scene game objects (use buckets? pre/post/final animation?)
		m_scene.Update(_fDeltaTime);
	}

	Shared<Camera> Game::GetPlayerCamera()
	{
		return m_player.GetCamera();
	}

	const Scene& Game::GetScene()
	{
		return m_scene;
	}

	const Player& Game::GetPlayer()
	{
		return m_player;
	}
}
