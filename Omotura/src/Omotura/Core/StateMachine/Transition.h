#pragma

#include "State.h"
#include "FuncPredicate.h"

namespace Omotura
{
	class Transition
	{
	private:
		Shared<State> m_pState;
		FuncPredicate m_condition;
	public:
		Transition() = default;
		Transition(Shared<State> _to, FuncPredicate _condition);
		~Transition() = default;

		Shared<State> GetState() const;
		FuncPredicate GetCondition() const;
	};
}