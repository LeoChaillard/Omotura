#include "StateMachine.h"

namespace Omotura
{
	void StateMachine::Update()
	{
		if (m_pCurrentNode == nullptr)
		{
			return;
		}

		Scope<Transition> nextTransition = GetNextTransition();
		if (nextTransition != nullptr)
		{
			ChangeState(nextTransition->GetState());
		}
		m_pCurrentNode->pState->Update();
	}

	void StateMachine::FixedUpdate()
	{
		if (m_pCurrentNode == nullptr)
		{
			return;
		}

		m_pCurrentNode->pState->FixedUpdate();
	}

	void StateMachine::SetState(Shared<State> _pState)
	{
		m_pCurrentNode = m_nodes[_pState->GetHandle()];
		m_pCurrentNode->pState->OnEnter();
	}

	void StateMachine::ChangeState(Shared<State> _pState)
	{
		if (_pState == m_pCurrentNode->pState)
		{
			return;
		}

		Shared<State> pCurrentState = m_pCurrentNode->pState;
		Shared<State> pNextState = _pState;
		
		pCurrentState->OnExit();
		pNextState->OnEnter();
		m_pCurrentNode = m_nodes[_pState->GetHandle()];
	}

	void StateMachine::AddTransition(Shared<State> _pFrom, Shared<State> _pTo, FuncPredicate _condition)
	{
		GetOrAddNode(_pFrom)->AddTransition(Transition(GetOrAddNode(_pTo)->pState, _condition));
	}
	
	void StateMachine::AddAnyTransition(Shared<State> _pTo, FuncPredicate _condition)
	{
		m_anyTransitions.push_back(Transition(_pTo, _condition));
	}

	Scope<Transition> StateMachine::GetNextTransition()
	{
		// Any transitions
		for (const Transition& transition : m_anyTransitions)
		{			
			if (transition.GetCondition().Evaluate())
			{
				return CreateScope<Transition>(transition);
			}
		}
		
		// Node transitions
		for (const Transition& transition : m_pCurrentNode->transitions)
		{
			if (transition.GetCondition().Evaluate())
			{
				return CreateScope<Transition>(transition);
			}
		}

		return nullptr;
	}

	Shared<StateNode> StateMachine::GetOrAddNode(Shared<State> _pState)
	{
		if (m_nodes.find(_pState->GetHandle()) == m_nodes.end())
		{
			Shared<StateNode> pNode = CreateShared<StateNode>(_pState);
			m_nodes.emplace(_pState->GetHandle(), pNode);
			return pNode;
		}
		else
		{
			return m_nodes[_pState->GetHandle()];
		}
		return nullptr;
	}
}