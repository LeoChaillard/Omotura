#include "ReloadState.h"

#include "../Game.h"

#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	extern Game g_gameInstance;

	ReloadState::ReloadState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
	}

	void ReloadState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_Reload", false);
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