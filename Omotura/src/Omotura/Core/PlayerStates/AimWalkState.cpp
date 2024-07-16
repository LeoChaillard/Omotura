#include "AimWalkState.h"

#include "../Game.h"

#include <string>


namespace Omotura
{
	AimWalkState::AimWalkState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void AimWalkState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_ADS_Walk");
	}

	void AimWalkState::Update()
	{

	}

	void AimWalkState::FixedUpdate()
	{

	}

	void AimWalkState::OnExit()
	{
		m_pPlayer->GetCurrentWeapon()->StopLoopingAnimation();
	}
}