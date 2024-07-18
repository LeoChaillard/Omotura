#include "Engine.h"

#include "Core/Camera.h"
#include "Core/Game.h"
#include "Core/Time.h"
#include "BackEnd/BackEnd.h"
#include "Asset/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererCommon.h"

#include "Input/Input.h"
#include "Input/KeyCodes.h"

namespace Omotura
{
	void Engine::Run()
	{
		BackEnd::Init(API::OPENGL);

		while (BackEnd::WindowIsOpen())
		{
			BackEnd::BeginFrame();
			BackEnd::UpdateSubSystems();

			if (!Game::GetInstance()->IsLoaded())
			{
				Game::GetInstance()->Create();
			}
			else
			{
				if (Input::KeyPressed(OMOTURA_KEY_F3))
				{
					Time::PauseOrResume();
				}

				if (!Time::IsPaused())
				{
					Game::GetInstance()->Update();
				}

				Renderer::RenderFrame();
			}

			BackEnd::EndFrame();
		}

		Renderer::Clear();
		BackEnd::Clear();
	}
}
