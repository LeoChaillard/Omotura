#include "AimIdleState.h"

#include "../Game.h"

#include <string>


namespace Omotura
{
	AimIdleState::AimIdleState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void AimIdleState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_ADS_Idle");
	}

	void AimIdleState::Update()
	{

	}

	void AimIdleState::FixedUpdate()
	{

	}

	void AimIdleState::OnExit()
	{
		m_pPlayer->GetCurrentWeapon()->StopLoopingAnimation();
	}
}