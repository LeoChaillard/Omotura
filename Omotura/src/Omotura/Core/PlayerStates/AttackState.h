#pragma once

#include "../StateMachine/State.h"
#include "../Player.h"

namespace Omotura
{
	class AttackState : public State
	{
	private:
		Shared<Player> m_pPlayer;

	public:
		AttackState();
		~AttackState() = default;

		virtual void OnEnter() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void OnExit() override;
	};
}