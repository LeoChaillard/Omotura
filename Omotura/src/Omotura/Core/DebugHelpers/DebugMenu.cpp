#include "DebugMenu.h"
#include "../Time.h"
#include "..//Game.h"

#include "../../Asset/AssetManager.h"
#include "../../Renderer/Font.h"
#include "../../Renderer/Renderer.h"
#include "../../Input/Input.h"
#include "../../Core/Terrain/Noise.h"

#include <glm/glm.hpp>
#include <imgui/imgui.h>

namespace Omotura
{
	extern Game g_gameInstance;

	void DebugMenu::Draw()
	{
		if (Time::IsPaused())
		{
			Input::ShowCursor();

			ImGui::SetNextWindowSize(ImVec2(250.0f, 0));
			ImGui::SetNextWindowPos(ImVec2(BackEnd::GetWindowWidth() / 20.0f, BackEnd::GetWindowHeight() / 20.0f));
			
			Shared<Font> pFont = AssetManager::GetAsset<Font>(hashID("arial"));
			ImGui::PushFont(pFont->ToImFont());
			ImGui::Begin("Debug Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::Separator();
			DrawRenderingMenu();
			DrawAnimationMenu();
			DrawPlayerMenu();
			DrawCameraMenu();
			DrawTerrainMenu();

			ImGui::End();
			ImGui::PopFont();
		}
	}

	void DebugMenu::DrawRenderingMenu()
	{
		if (ImGui::BeginMenu("Rendering..."))
		{
			if (ImGui::BeginMenu("Mesh Options..."))
			{
				if (ImGui::BeginMenu("Debug Draw Options..."))
				{					
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Light Options..."))
			{				
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
	}

	void DebugMenu::DrawAnimationMenu()
	{
		if (ImGui::BeginMenu("Animation..."))
		{
			ImGui::EndMenu();
		}
	}

	void DebugMenu::DrawPlayerMenu()
	{
		if (ImGui::BeginMenu("Player Menu..."))
		{
			if (ImGui::MenuItem("Enable Fly"))
			{
				g_gameInstance.GetPlayer()->m_bFly = true;
				g_gameInstance.GetPlayer()->GetAnimator()->SetEnabled(false);
				Time::PauseOrResume();
			}
			if (ImGui::MenuItem("Disable Fly"))
			{
				g_gameInstance.GetPlayer()->m_bFly = false;
				g_gameInstance.GetPlayer()->GetAnimator()->SetEnabled(true);
				Time::PauseOrResume();
			}

			// Speed
			static int iSpeed = 20;
			ImGui::SliderInt("Speed", &iSpeed, 20, 100);
			g_gameInstance.GetPlayer()->SetSpeed((float)iSpeed);

			ImGui::EndMenu();
		}
	}

	void DebugMenu::DrawCameraMenu()
	{
		if (ImGui::BeginMenu("Camera..."))
		{
			ImGui::EndMenu();
		}
	}

	void DebugMenu::DrawTerrainMenu()
	{
		if (ImGui::BeginMenu("Terrain..."))
		{
			if (ImGui::BeginMenu("Noise Options..."))
			{
				// Map Width / Map Height 
				static int iWidth = 0, iHeight = 0;
				ImGui::SliderInt("Map Width", &iWidth, 0, 2000);
				ImGui::SliderInt("Map Height", &iHeight, 0, 2000);

				// Grid Size / Scale
				static int iGridSize = 0;
				ImGui::SliderInt("Grid Size / Scale", &iGridSize, 0, 2000);

				// Octaves
				static int iOctaves = 0;
				ImGui::SliderInt("Octaves", &iOctaves, 0, 20);

				// Persistance
				static float fPersistance = 0.0f;
				ImGui::SliderFloat("Persistance", &fPersistance, 0.0f, 1.0f);
				
				// Lacunarity
				static float fLacunarity = 0.0f;
				ImGui::SliderFloat("Lacunarity", &fLacunarity, 0.0f, 20.0f);

				// Seed
				static int iSeed = 0;
				ImGui::SliderInt("Seed", &iSeed, 0, 20);

				// Generate Map	
				if (ImGui::Button("Generate Terrain"))
				{
					Shared<Terrain> pTerrain = g_gameInstance.GetScene().GetTerrain();
					pTerrain->GenerateTerrain(iWidth, iHeight, iSeed, iGridSize, iOctaves, fPersistance, fLacunarity);
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
	}

}