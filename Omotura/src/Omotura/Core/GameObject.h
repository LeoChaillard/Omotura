#pragma once

#include "Transform.h"
#include "Animator.h"

#include <cyclone/body.h>

using namespace cyclone;

namespace Omotura
{
	class GameObject
	{
	protected:
		Transform m_transform;

		// Components
		std::unique_ptr<Animator> m_pAnimController;
		std::unique_ptr<RigidBody> m_pRigidBody;		

	public:
		GameObject();
	};
}