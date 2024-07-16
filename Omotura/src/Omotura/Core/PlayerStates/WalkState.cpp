#include "WalkState.h"

#include "../Game.h"

#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	WalkState::WalkState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void WalkState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_Walk");
		Audio::Play("player_step_1", 0.1f, false);
	}

	void WalkState::Update()
	{		

	}

	void WalkState::FixedUpdate()
	{
		
	}

	void WalkState::OnExit()
	{
		m_pPlayer->GetCurrentWeapon()->StopLoopingAnimation();
		//Audio::Stop("player_step_1");
	}
}