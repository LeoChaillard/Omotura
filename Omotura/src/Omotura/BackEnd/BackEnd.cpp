#include "BackEnd.h"

#include "../Asset/AssetManager.h"
#include "../Utils/Utils.hpp"

#include <iostream>
#include <string>

namespace Omotura
{
	namespace constants
	{
		constexpr unsigned int iWidth = 1920;
		constexpr unsigned int iHeight = 1080;
	}

	GLFWwindow* BackEnd::m_pWindow;
	API BackEnd::m_api;

	void BackEnd::Init(API _api)
	{
		// API
		m_api = _api;

		if (m_api == API::OPENGL)
		{
			glfwInit();

			// Window
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Create Winow
			CreateGLFWWindow();
			glfwMakeContextCurrent(m_pWindow);
			gladLoadGL();
			glViewport(0, 0, constants::iWidth, constants::iHeight);
		}
		
		// Init Subsystems (Input, Audio, Physics...)
		AssetManager::Init();

	}

	const API& BackEnd::GetAPI()
	{
		return m_api;
	}

	void BackEnd::CreateGLFWWindow()
	{
		m_pWindow = glfwCreateWindow(constants::iWidth, constants::iHeight, Utils::GetEngineName().c_str(), NULL, NULL);
		if (m_pWindow == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
	}

	bool BackEnd::WindowIsOpen()
	{
		return !glfwWindowShouldClose(m_pWindow);
	}

	GLFWwindow* BackEnd::GetGLFWwindow()
	{
		return m_pWindow;
	}

	int BackEnd::GetWindowWidth()
	{
		return constants::iWidth;
	}

	int BackEnd::GetWindowHeight()
	{
		return constants::iHeight;
	}

	void BackEnd::BeginFrame()
	{
		glfwPollEvents();
	}

	void BackEnd::EndFrame()
	{
		glfwSwapBuffers(m_pWindow);
	}

	void BackEnd::Clear()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}
}