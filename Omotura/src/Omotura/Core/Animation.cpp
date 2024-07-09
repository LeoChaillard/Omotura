#include "Animation.h"

namespace Omotura
{
	Animation::Animation()
		: m_fDuration(),
		m_fTicksPerSecond(),
		m_fFinalTimeStamp(),
		m_bLoop(false),
		m_bPaused(false),
		m_fSpeed(1.0f)
	{
	}

	Animation::Animation(float _fDuration, float _fTicksPerSecond, float _fFinalTimeStamp)
		: m_fDuration(_fDuration),
		m_fTicksPerSecond(_fTicksPerSecond),
		m_fFinalTimeStamp(_fFinalTimeStamp)
	{
	}

	void Animation::SetDuration(float _fDuration)
	{
		m_fDuration = _fDuration;
	}

	const float& Animation::GetDuration() const
	{
		return m_fDuration;
	}

	void Animation::SetTicksPerSecond(float _fTicks)
	{
		m_fTicksPerSecond = _fTicks;
	}

	const float& Animation::GetTicksPerSecond() const
	{
		return m_fTicksPerSecond;
	}

	void Animation::SetFinalTimeStamp(float _fTimeStamp)
	{
		m_fFinalTimeStamp = _fTimeStamp;
	}

	void Animation::AddNode(const AnimatedNode& _animationNode)
	{
		m_vAnimationNodes.push_back(_animationNode);
	}

	const std::vector<AnimatedNode>& Animation::GetAnimatedNodes() const
	{
		return m_vAnimationNodes;
	}

	bool Animation::IsLooping()
	{
		return m_bLoop;
	}

	bool Animation::IsPaused()
	{
		return m_bPaused;
	}

	float Animation::GetSpeed()
	{
		return m_fSpeed;
	}

	void Animation::SetLooping(bool _bLoop)
	{
		m_bLoop = _bLoop;
	}

	void Animation::SetPaused(bool _bPaused)
	{
		m_bPaused = _bPaused;
	}

	void Animation::SetSpeed(float _fSpeed)
	{
		m_fSpeed = _fSpeed;
	}
}