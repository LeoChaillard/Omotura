#include "Transition.h"

namespace Omotura
{
	Transition::Transition(Shared<State> _to, FuncPredicate _condition)
		: m_pState(_to),
		m_condition(_condition)
	{
	}

	Shared<State> Transition::GetState() const
	{
		return m_pState;
	}

	FuncPredicate Transition::GetCondition() const
	{
		return m_condition;
	}
}