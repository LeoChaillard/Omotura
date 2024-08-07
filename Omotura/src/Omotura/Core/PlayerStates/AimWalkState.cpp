#include "AimWalkState.h"

#include "../Game.h"

#include <string>


namespace Omotura
{
	extern Game g_gameInstance;

	AimWalkState::AimWalkState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
	}

	void AimWalkState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_ADS_Walk");
	}

	void AimWalkState::Update()
	{

	}

	void AimWalkState::FixedUpdate()
	{

	}

	void AimWalkState::OnExit()
	{
		g_gameInstance.GetPlayer()->GetAnimator()->StopLoopingAnimation();
	}
}