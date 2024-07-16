#include "AimTransitionState.h"

#include "../Game.h"

#include <string>


namespace Omotura
{
	AimTransitionState::AimTransitionState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void AimTransitionState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_ADS_In", false, 3.0f);
	}

	void AimTransitionState::Update()
	{

	}

	void AimTransitionState::FixedUpdate()
	{

	}

	void AimTransitionState::OnExit()
	{

	}
}