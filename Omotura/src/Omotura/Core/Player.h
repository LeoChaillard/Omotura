#pragma once

#include "FlyCamera.h"
#include "FPSCamera.h"

namespace Omotura
{
	class Player
	{
	private:
		Shared<Camera> m_pCamera;
		glm::mat4 m_weaponViewMatrix;

	public:
		Player();
		~Player() = default;

		void Update(float _fDeltaTime);
		void FixedUpdate(float _fFixedDeltaTime);

		Shared<Camera> GetCamera();
		void SetWeaponViewMatrix(const glm::mat4& _weaponViewMatrix);
	};
}