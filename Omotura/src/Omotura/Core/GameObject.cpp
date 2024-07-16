#include "GameObject.h"

namespace Omotura
{
	GameObject::GameObject()
		: m_bVisible(true)
	{
		m_pAnimator = CreateShared<Animator>();
		m_pRigidBody = CreateShared<RigidBody>();
	}

	void GameObject::Hide()
	{
		m_bVisible = false;
	}

	void GameObject::Show()
	{
		m_bVisible = true;
	}

	bool GameObject::IsHidden()
	{
		return !m_bVisible;
	}
}
