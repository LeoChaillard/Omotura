#pragma once

#include "../StateMachine/State.h"
#include "../Player.h"

namespace Omotura
{
	class DrawState : public State
	{
	private:
		Shared<Player> m_pPlayer;

	public:
		DrawState();
		~DrawState() = default;

		virtual void OnEnter() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void OnExit() override;
	};
}