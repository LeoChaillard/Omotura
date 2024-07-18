#include "Time.h"

#include <GLFW/glfw3.h>

namespace Omotura
{
	namespace constants
	{
		constexpr float _fFixedDeltaTime = 1.0 / 60.0f;
	}

	float Time::m_fLastFrame;
	float Time::m_fCurrentFrame;
	bool Time::m_bPaused;

	void Time::Update()
	{
		if (!m_bPaused)
		{
			m_fLastFrame = m_fCurrentFrame;
			m_fCurrentFrame = glfwGetTime();
		}
	}

	float Time::GetCurrentTime()
	{
		return m_fCurrentFrame;
	}

	float Time::GetDeltaTime()
	{
		return m_fCurrentFrame - m_fLastFrame;
	}

	float Time::GetFixedDeltaTime()
	{
		return constants::_fFixedDeltaTime;
	}

	void Time::Pause()
	{
		m_bPaused = true;
	}

	void Time::Resume()
	{
		m_bPaused = false;
	}

	void Time::PauseOrResume()
	{
		m_bPaused = !m_bPaused;
	}

	bool Time::IsPaused()
	{
		return m_bPaused;
	}

}