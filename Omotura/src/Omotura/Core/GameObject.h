#pragma once

#include "Transform.h"
#include "Animator.h"

#include "../Common/Common.h"

#include <cyclone/body.h>

using namespace cyclone;

namespace Omotura
{
	class GameObject
	{
	private:
		bool m_bVisible;

	protected:
		Transform m_transform;

		// Components
		Shared<Animator> m_pAnimator;
		Shared<RigidBody> m_pRigidBody;		

	public:
		GameObject();

		void Show();
		void Hide();
		bool IsVisible();
	};
}