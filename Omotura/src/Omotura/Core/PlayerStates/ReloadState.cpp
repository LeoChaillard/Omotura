#include "ReloadState.h"

#include "../Game.h"

#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	ReloadState::ReloadState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void ReloadState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_Reload", false);
		Audio::Play(strWeaponName + "_Reload", 0.1f, false);
	}

	void ReloadState::Update()
	{

	}

	void ReloadState::FixedUpdate()
	{

	}

	void ReloadState::OnExit()
	{

	}
}