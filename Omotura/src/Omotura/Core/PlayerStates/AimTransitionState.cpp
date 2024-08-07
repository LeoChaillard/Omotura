#include "AimTransitionState.h"

#include "../Game.h"

#include <string>


namespace Omotura
{
	extern Game g_gameInstance;

	AimTransitionState::AimTransitionState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
	}

	void AimTransitionState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_ADS_In", false, 3.0f);
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