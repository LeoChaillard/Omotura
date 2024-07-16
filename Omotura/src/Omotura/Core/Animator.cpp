#include "Animator.h"
#include "AnimatorManager.h"

namespace Omotura
{
	Animator::Animator()
	{
		m_pDefaultState = nullptr;
		m_pAnimationStates = CreateShared<StateMachine>();
	}

	void Animator::Update()
	{
		m_pAnimationStates->Update();
	}

	void Animator::FixedUpdate()
	{
		m_pAnimationStates->FixedUpdate();
	}

	void Animator::Register()
	{
		AnimatorManager::Register(CreateShared<Animator>(*this));
	}

	void Animator::At(Shared<State> _pFrom, Shared<State> _pTo, FuncPredicate _condition)
	{
		m_pAnimationStates->AddTransition(_pFrom, _pTo, _condition);
	}

	void Animator::Any(Shared<State> _pTo, FuncPredicate _condition)
	{
		m_pAnimationStates->AddAnyTransition(_pTo, _condition);
	}

	void Animator::SetDefaultState(Shared<State> _pState)
	{
		m_pAnimationStates->SetState(_pState);
		m_pDefaultState = _pState;
	}

	void Animator::ResetToDefault()
	{
		if (m_pDefaultState != nullptr)
		{
			m_pAnimationStates->SetState(m_pDefaultState);
		}
	}
}