#pragma once

#include "State.h"
#include "Transition.h"

#include <vector>
#include <map>

namespace Omotura
{
	struct StateNode
	{
		Shared<State> pState;
		std::vector<Transition> transitions;

		StateNode()
		{
			pState = nullptr;
			transitions = std::vector<Transition>();
		}

		StateNode(Shared<State> _pState)
		{
			pState = _pState;
			transitions = std::vector<Transition>();
		}

		void AddTransition(const Transition& _transition)
		{
			transitions.push_back(_transition);
		}
	};

	class StateMachine
	{
	private:
		Shared<StateNode> m_pCurrentNode;
		std::map<StateHandle, Shared<StateNode>> m_nodes;
		std::vector<Transition> m_anyTransitions;

	public:
		StateMachine() = default;
		~StateMachine() = default;

		void Update();
		void FixedUpdate();

		void SetState(Shared<State> _pState);
		void AddTransition(Shared<State> _pFrom, Shared<State> _pTo, FuncPredicate _condition);
		void AddAnyTransition(Shared<State> _pTo, FuncPredicate _condition);

	private:
		void ChangeState(Shared<State> _pState);
		Scope<Transition> GetNextTransition();
		Shared<StateNode> GetOrAddNode(Shared<State> _pState);
	};
}