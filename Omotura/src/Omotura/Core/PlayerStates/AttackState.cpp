#include "AttackState.h"

#include "../Game.h"

#include "../../Audio/Audio.h"

#include <random>
#include <string>

namespace Omotura
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(1.0, 3.0);

	AttackState::AttackState()
	{
		m_pPlayer = Game::GetInstance()->GetPlayer();
	}

	void AttackState::OnEnter()
	{
		int iNum = dist(mt);
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		m_pPlayer->GetCurrentWeapon()->SetAnimation(strWeaponName + "_Fire" + std::to_string(iNum), false, 3.0f);
		Audio::Play(strWeaponName + "_Fire" + std::to_string(iNum), 0.1f, false);
	}

	void AttackState::Update()
	{

	}

	void AttackState::FixedUpdate()
	{

	}

	void AttackState::OnExit()
	{

	}
}