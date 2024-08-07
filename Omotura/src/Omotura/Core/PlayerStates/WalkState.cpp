#include "WalkState.h"

#include "../Game.h"

#include "../../Audio/Audio.h"

#include <string>

namespace Omotura
{
	extern Game g_gameInstance;

	namespace constants
	{
		const char* pDefaultStepFile = "player_step_1";
	}

	WalkState::WalkState()
	{
		m_pPlayer = g_gameInstance.GetPlayer();
		m_strFileName = std::string(constants::pDefaultStepFile);
		std::random_device rd;
		mt = std::mt19937(rd());
		dist = std::uniform_real_distribution<double>(1.0, 5.0);
	}

	void WalkState::OnEnter()
	{
		std::string strWeaponName = m_pPlayer->GetCurrentWeapon()->m_strName;
		g_gameInstance.GetPlayer()->GetAnimator()->SetAnimation(strWeaponName + "_Walk");
		Audio::Play(m_strFileName, 0.01f, false);
	}

	void WalkState::Update()
	{		
		if (Audio::IsAudioFinished(m_strFileName))
		{
			int iNum = (int)dist(mt);
			m_strFileName = "player_step_" + std::to_string(iNum);
			Audio::Play(m_strFileName, 0.01f, false);
		}
	}

	void WalkState::FixedUpdate()
	{
		
	}

	void WalkState::OnExit()
	{
		g_gameInstance.GetPlayer()->GetAnimator()->StopLoopingAnimation();
	}
}