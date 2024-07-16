#pragma once

#include "../StateMachine/State.h"
#include "../Player.h"

namespace Omotura
{
	class ReloadState : public State
	{
	private:
		Shared<Player> m_pPlayer;

	public:
		ReloadState();
		~ReloadState() = default;

		virtual void OnEnter() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void OnExit() override;
	};
}