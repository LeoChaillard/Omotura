#include "IdleState.h"

#include "../Game.h"

#include <string>

namespace Omotura
{
	extern Game g_gameInstance;

	IdleState::IdleState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
	}

	void IdleState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_Idle");
	}

	void IdleState::Update()
	{

	}

	void IdleState::FixedUpdate()
	{

	}

	void IdleState::OnExit()
	{
		g_gameInstance.GetPlayer()->GetAnimator()->StopLoopingAnimation();
	}
}