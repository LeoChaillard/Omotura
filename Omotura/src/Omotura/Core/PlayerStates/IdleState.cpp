#include "IdleState.h"

#include "../Game.h"

#include <string>

namespace Omotura
{
	IdleState::IdleState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void IdleState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_Idle");
	}

	void IdleState::Update()
	{

	}

	void IdleState::FixedUpdate()
	{

	}

	void IdleState::OnExit()
	{
		m_pPlayer->GetCurrentWeapon()->StopLoopingAnimation();
	}
}