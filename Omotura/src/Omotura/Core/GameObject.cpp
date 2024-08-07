#include "GameObject.h"

namespace Omotura
{
	GameObject::GameObject()
		: m_bVisible(true)
	{
		m_pAnimator = CreateShared<Animator>();
		m_pRigidBody = CreateShared<RigidBody>();
	}

	void GameObject::Show()
	{
		m_bVisible = true;
	}

	void GameObject::Hide()
	{
		m_bVisible = false;
	}

	bool GameObject::IsVisible()
	{
		return m_bVisible;
	}

}
