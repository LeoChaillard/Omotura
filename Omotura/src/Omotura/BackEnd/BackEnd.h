#pragma once

#include "../Common/Common.h"
#include "../Renderer/RendererCommon.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Omotura
{
	class BackEnd
	{
	private:
		static GLFWwindow* m_pWindow;
		static API m_api;

	public:
		static void Init(API _api);
		static const API& GetAPI();
		static void Clear();
		static void BeginFrame();
		static void EndFrame();

		// Window
		static void CreateGLFWWindow();
		static bool WindowIsOpen();
		static GLFWwindow* GetGLFWwindow();
		static int GetWindowWidth();
		static int GetWindowHeight();
	};
}