#pragma once

namespace Omotura
{
	class Time
	{
	private:
		static float m_fLastFrame;
		static float m_fCurrentFrame;
		static bool m_bPaused;

	public:
		static void Update();

		static float GetCurrentTime();
		static float GetDeltaTime();
		static float GetFixedDeltaTime();

		static void Pause();
		static void Resume();
		static void PauseOrResume();
		static bool IsPaused();
	};
}