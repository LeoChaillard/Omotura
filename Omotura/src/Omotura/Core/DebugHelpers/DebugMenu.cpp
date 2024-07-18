#include "DebugMenu.h"
#include "../Time.h"
#include "..//Game.h"

#include "../../BackEnd/BackEnd.h"
#include "../../Input/Input.h"

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Omotura
{
	void DebugMenu::Draw()
	{
		if (Time::IsPaused())
		{
			Input::ShowCursor();
			ImGui::SetNextWindowPos(ImVec2(BackEnd::GetWindowWidth() / 20.0f, BackEnd::GetWindowHeight() / 20.0f));
			ImGui::Begin("Debug Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

			DrawRenderingMenu();
			DrawAnimationMenu();
			DrawPlayerMenu();

			ImGui::End();
		}
	}

	void DebugMenu::DrawRenderingMenu()
	{
		if (ImGui::BeginMenu("Rendering..."))
		{
			ImGui::MenuItem("Mesh Options");
			ImGui::MenuItem("Ligting");
			ImGui::MenuItem("Shadows");
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
				Game::GetInstance()->GetPlayer()->MultiplySpeed(0.5f);
				Game::GetInstance()->GetPlayer()->GetAnimator()->SetEnabled(true);
				Time::PauseOrResume();
			}
			ImGui::EndMenu();
		}
	}
}