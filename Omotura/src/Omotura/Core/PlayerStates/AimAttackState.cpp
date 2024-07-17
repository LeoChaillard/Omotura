#include "AimAttackState.h"

#include "../Game.h"
#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	AimAttackState::AimAttackState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
		std::random_device rd;
		mt = std::mt19937(rd());
		dist = std::uniform_real_distribution<double>(1.0, 3.0);
	}

	void AimAttackState::OnEnter()
	{
		int iNum = dist(mt);
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_ADS_Fire" + std::to_string(iNum), false, 3.0f);
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