#pragma once

#include "StateMachine/StateMachine.h"

#include "UUID.h"

namespace Omotura
{
	using AnimatorHandle = UUID;
	class Animator
	{
	private:
		Shared<State> m_pDefaultState;
		Shared<StateMachine> m_pAnimationStates;
		
		// Debug
		bool m_bEnabled = true;

	public:
		Animator();
		~Animator() = default;

		void Update();
		void FixedUpdate();

		void At(Shared<State> _pFrom, Shared<State> _pTo, FuncPredicate _condition);
		void Any(Shared<State> _pTo, FuncPredicate _condition);

		void SetDefaultState(Shared<State> _pState);
		void ResetToDefault();

		void SetEnabled(bool _bEnabled);
		bool IsEnabled();
	};
}