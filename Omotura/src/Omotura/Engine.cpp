#include "Engine.h"

#include "Core/Camera.h"
#include "Core/Game.h"
#include "BackEnd/BackEnd.h"
#include "Asset/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererCommon.h"

#include <glad/glad.h>

namespace Omotura
{
	float g_fLastFrame = glfwGetTime();
	float g_fCurrentFrame = g_fLastFrame;

	void Engine::Run()
	{
		BackEnd::Init(API::OPENGL);

		while (BackEnd::WindowIsOpen())
		{
			BackEnd::BeginFrame();

			if (!Game::GetInstance()->IsLoaded())
			{
				Game::GetInstance()->Create();
			}
			else
			{
				// Update time
				g_fLastFrame = g_fCurrentFrame;
				g_fCurrentFrame = glfwGetTime();
				float fDeltaTime = g_fCurrentFrame - g_fLastFrame;

				Game::GetInstance()->Update(fDeltaTime);
				Renderer::RenderFrame();
			}
					
			BackEnd::EndFrame();
		}

		Renderer::Clear();
		BackEnd::Clear();
	}
}
