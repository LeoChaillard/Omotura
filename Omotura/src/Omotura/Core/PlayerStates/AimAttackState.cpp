#include "AimAttackState.h"

#include "../Game.h"
#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	extern Game g_gameInstance;

	AimAttackState::AimAttackState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
		std::random_device rd;
		mt = std::mt19937(rd());
		dist = std::uniform_real_distribution<double>(1.0, 3.0);
	}

	void AimAttackState::OnEnter()
	{
		int iNum = (int)dist(mt);
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_ADS_Fire" + std::to_string(iNum), false, 3.0f);
		Audio::Play(strWeaponName + "_Fire" + std::to_string(iNum), 0.1f, false);
	}

	void AimAttackState::Update()
	{

	}

	void AimAttackState::FixedUpdate()
	{

	}

	void AimAttackState::OnExit()
	{
	}
}