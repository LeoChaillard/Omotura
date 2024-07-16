#pragma once

#include "../StateMachine/State.h"
#include "../Player.h"

#include <random>

namespace Omotura
{
	class AimAttackState : public State
	{
	private:
		Shared<Player> m_pPlayer;

		std::random_device rd;
		std::mt19937 mt;
		std::uniform_real_distribution<double> dist;

	public:
		AimAttackState();
		~AimAttackState() = default;

		virtual void OnEnter() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void OnExit() override;
	};
}