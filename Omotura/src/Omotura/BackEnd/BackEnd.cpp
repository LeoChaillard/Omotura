#include "BackEnd.h"

#include "../Asset/AssetManager.h"
#include "../Input/Input.h"
#include "../Audio/Audio.h"
#include "../Utils/Utils.hpp"

#include <iostream>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Create Window
			CreateGLFWWindow();
			glfwMakeContextCurrent(m_pWindow);
			gladLoadGL();
			glViewport(0, 0, constants::iWidth, constants::iHeight);

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
			ImGui_ImplOpenGL3_Init("#version 330");

			// Load default font
			io.Fonts->AddFontDefault();

			// Setup style parameters
			ImGui::GetStyle().WindowMinSize = ImVec2(250.0f, 100.0f);

			// Setup colors
			ImVec4* pColors = ImGui::GetStyle().Colors;
			pColors[ImGuiCol_Text] = ImVec4(200.0f, 200.0f, 200.0f, 1.0f);
			pColors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
			pColors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
			pColors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
			pColors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
			pColors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
		}
		
		// Init Subsystems (Input, Audio, Physics...)
		Audio::Init();
		Input::Init();
		AssetManager::Init();
	}

	void BackEnd::UpdateSubSystems()
	{
		Audio::Update();
		Input::Update();
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
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Poll GLFW events
		glfwPollEvents();
	}

	void BackEnd::EndFrame()
	{
		// End Dear ImGui frame
		ImGui::EndFrame();

		// Swap GLFW buffers
		glfwSwapBuffers(m_pWindow);
	}

	void BackEnd::Clear()
	{
		// Clean up Dear ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Clean up GLFW
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}
}