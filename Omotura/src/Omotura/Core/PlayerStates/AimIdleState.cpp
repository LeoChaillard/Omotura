#include "AimIdleState.h"

#include "../Game.h"

#include <string>


namespace Omotura
{
	extern Game g_gameInstance;

	AimIdleState::AimIdleState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
	}

	void AimIdleState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_ADS_Idle");
	}

	void AimIdleState::Update()
	{

	}

	void AimIdleState::FixedUpdate()
	{

	}

	void AimIdleState::OnExit()
	{
		g_gameInstance.GetPlayer()->GetAnimator()->StopLoopingAnimation();
	}
}