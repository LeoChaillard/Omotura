#include "DrawState.h"

#include "../Game.h"

#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	DrawState::DrawState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void DrawState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_Draw", false);
		Audio::Play(strWeaponName + "_FirstDraw", 0.1f, false);
	}

	void DrawState::Update()
	{

	}

	void DrawState::FixedUpdate()
	{

	}

	void DrawState::OnExit()
	{

	}
}