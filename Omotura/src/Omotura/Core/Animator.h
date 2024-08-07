#pragma once

#include "UUID.h"

#include "StateMachine/StateMachine.h"

#include "../Core/Animation.h"
#include "../Renderer/RendererCommon.h"

namespace Omotura
{
	class Animator
	{
	private:
		Shared<State> m_pDefaultState;
		Shared<StateMachine> m_pAnimationStates;

		std::string m_strAnimatedMesh;
		
		// Debug
		bool m_bEnabled = true;

		// Animation related
		Shared<Animation> m_pCurrentAnimation;
		float m_fCurrentAnimationTime;
		bool m_bCurrentAnimationFinished;

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

		// Animation related
		void UpdateAnimation(float _fDeltaTime);
		Shared<AnimatedNode> FindAnimatedNode(BoneHandle _handle);
		float GetAnimationTime();

		void SetAnimatedMesh(std::string _strAnimatedMesh);
		const std::string& GetAnimatedMesh();
		void SetAnimation(const std::string& _strAnimation, bool _bLooping = true, float _fSpeed = 1.0f);
		bool CurrentAnimationFinished();
		void StopLoopingAnimation();
		void ResetAnimation();
	};
}