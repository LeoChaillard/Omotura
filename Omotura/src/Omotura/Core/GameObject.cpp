#include "GameObject.h"

namespace Omotura
{
	GameObject::GameObject()
	{
		m_pAnimController = std::make_unique<Animator>();
		m_pRigidBody = std::make_unique<RigidBody>();
	}
}
