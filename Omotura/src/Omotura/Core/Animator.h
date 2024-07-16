#pragma once

#include "StateMachine/StateMachine.h"

namespace Omotura
{
	class Animator
	{
	private:
		Shared<State> m_pDefaultState;
		Shared<StateMachine> m_pAnimationStates;

	public:
		Animator();
		~Animator() = default;

		void Update();
		void FixedUpdate();

		void Register();

		void At(Shared<State> _pFrom, Shared<State> _pTo, FuncPredicate _condition);
		void Any(Shared<State> _pTo, FuncPredicate _condition);

		void SetDefaultState(Shared<State> _pState);
		void ResetToDefault();
	};
}