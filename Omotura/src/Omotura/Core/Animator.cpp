#include "Animator.h"
#include "AnimatorManager.h"

#include "../Asset/AssetManager.h"

namespace Omotura
{
	Animator::Animator()
		: m_pCurrentAnimation(),
		m_fCurrentAnimationTime(),
		m_bCurrentAnimationFinished(false)
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

	void Animator::SetEnabled(bool _bEnabled)
	{
		m_bEnabled = _bEnabled;
	}

	bool Animator::IsEnabled()
	{
		return m_bEnabled;
	}

	/*******************ANIMATION RELATED************************/
	void Animator::UpdateAnimation(float _fDeltaTime)
	{
		float fDuration = m_pCurrentAnimation->GetDuration() / m_pCurrentAnimation->GetTicksPerSecond();

		// Increase the animation time
		if (!m_pCurrentAnimation->IsPaused())
		{
			m_fCurrentAnimationTime += _fDeltaTime * m_pCurrentAnimation->GetSpeed();
		}

		// Animation is complete?
		if (m_fCurrentAnimationTime > fDuration)
		{
			if (!m_pCurrentAnimation->IsLooping())
			{
				m_bCurrentAnimationFinished = true;
				m_fCurrentAnimationTime = fDuration;
				m_pCurrentAnimation->SetPaused(true);
			}
			else
			{
				m_fCurrentAnimationTime = 0.0f;
			}
		}
	}

	Shared<AnimatedNode> Animator::FindAnimatedNode(BoneHandle _handle)
	{
		const std::vector<AnimatedNode>& vNodes = m_pCurrentAnimation->GetAnimatedNodes();
		int iNodes = (int)vNodes.size();
		for (int i = 0; i < iNodes; i++)
		{
			if (vNodes[i].handle == _handle)
			{
				Shared<AnimatedNode> pNode = CreateShared<AnimatedNode>(vNodes[i]);
				return pNode;
			}
		}
		return nullptr;
	}

	float Animator::GetAnimationTime()
	{
		float fTicksPerSecond = m_pCurrentAnimation->GetTicksPerSecond() != 0 ? m_pCurrentAnimation->GetTicksPerSecond() : 25.0f;
		float fTimeInTicks = m_fCurrentAnimationTime * fTicksPerSecond;
		float fAnimationTime = fmod(fTimeInTicks, m_pCurrentAnimation->GetDuration());
		return std::min(fTimeInTicks, m_pCurrentAnimation->GetDuration());
	}

	void Animator::SetAnimatedMesh(std::string _strAnimatedMesh)
	{
		m_strAnimatedMesh = _strAnimatedMesh;
	}

	const std::string& Animator::GetAnimatedMesh()
	{
		return m_strAnimatedMesh;
	}

	void Animator::SetAnimation(const std::string& _strAnimation, bool _bLooping /*= true*/, float _fSpeed /*= 1.0f*/)
	{
		Shared<Animation> pAnimation = AssetManager::GetAsset<Animation>(hashID(_strAnimation.c_str()));
		if (pAnimation)
		{
			m_pCurrentAnimation = pAnimation;
			m_pCurrentAnimation->SetLooping(_bLooping);
			m_pCurrentAnimation->SetPaused(false);
			m_pCurrentAnimation->SetSpeed(_fSpeed);
			m_bCurrentAnimationFinished = false;
		}
	}

	bool Animator::CurrentAnimationFinished()
	{
		return m_bCurrentAnimationFinished;
	}

	void Animator::StopLoopingAnimation()
	{
		m_bCurrentAnimationFinished = true;
		m_fCurrentAnimationTime = 0.0f;
		m_pCurrentAnimation->SetPaused(true);
	}

	void Animator::ResetAnimation()
	{
		m_bCurrentAnimationFinished = false;
		m_fCurrentAnimationTime = 0.0f;
		m_pCurrentAnimation->SetPaused(false);
	}
}
