#pragma

#include "Animator.h"

#include <vector>

namespace Omotura
{
	class AnimatorManager
	{
	private:
		static std::vector<Shared<Animator>> m_vRegisteredAnimators;

	public:
		static void Register(Shared<Animator> _pAnimator);

		static void Update();
		static void FixedUpdate();
	};
}