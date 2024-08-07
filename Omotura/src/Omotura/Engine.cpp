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
	extern Game g_gameInstance;

	void Engine::Run()
	{
		BackEnd::Init(API::OPENGL);
		g_gameInstance.Init();
		Renderer::Init();

		while (BackEnd::WindowIsOpen())
		{
			BackEnd::BeginFrame();
			BackEnd::UpdateSubSystems();

			if (Input::KeyPressed(OMOTURA_KEY_F3))
			{
				Time::PauseOrResume();
			}

			if (!Time::IsPaused())
			{
				g_gameInstance.Update();
			}

			Renderer::RenderFrame();			

			BackEnd::EndFrame();
		}

		Renderer::Clear();
		BackEnd::Clear();
	}
}
