#include "DrawState.h"

#include "../Game.h"

#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	extern Game g_gameInstance;

	DrawState::DrawState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
	}

	void DrawState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_Draw", false);
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