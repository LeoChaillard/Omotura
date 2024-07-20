#include "DebugMenu.h"
#include "../Time.h"
#include "..//Game.h"

#include "../../Asset/AssetManager.h"
#include "../../Renderer/Font.h"
#include "../../Input/Input.h"

#include <glm/glm.hpp>
#include <imgui/imgui.h>

namespace Omotura
{
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
				Game::GetInstance()->GetPlayer()->m_bFly = true;
				Game::GetInstance()->GetPlayer()->MultiplySpeed(2.0f);
				Game::GetInstance()->GetPlayer()->GetAnimator()->SetEnabled(false);
				Time::PauseOrResume();
			}
			if (ImGui::MenuItem("Disable Fly"))
			{
				Game::GetInstance()->GetPlayer()->m_bFly = false;
				Game::GetInstance()->GetPlayer()->SetDefaultSpeed();
				Game::GetInstance()->GetPlayer()->GetAnimator()->SetEnabled(true);
				Time::PauseOrResume();
			}
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

}