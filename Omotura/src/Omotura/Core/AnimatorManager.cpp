#include "AnimatorManager.h"

namespace Omotura
{
	std::vector<Shared<Animator>> AnimatorManager::m_vRegisteredAnimators;

	void AnimatorManager::Update()
	{
		int iSize = (int)m_vRegisteredAnimators.size();
		for (int i = 0; i < iSize; i++)
		{
			if (m_vRegisteredAnimators[i]->IsEnabled())
			{
				m_vRegisteredAnimators[i]->Update();
			}
		}
	}

	void AnimatorManager::FixedUpdate()
	{
		int iSize = (int)m_vRegisteredAnimators.size();
		for (int i = 0; i < iSize; i++)
		{			
			if (m_vRegisteredAnimators[i]->IsEnabled())
			{
				m_vRegisteredAnimators[i]->FixedUpdate();
			}
		}
	}

	void AnimatorManager::Register(Shared<Animator> _pAnimator)
	{
		m_vRegisteredAnimators.push_back(_pAnimator);
	}
}