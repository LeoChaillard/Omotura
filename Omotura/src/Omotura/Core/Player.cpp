#include "Player.h"

#include "../BackEnd/BackEnd.h"

namespace Omotura
{
	Player::Player()
		: m_weaponViewMatrix(glm::mat4(1.0f))
	{
		m_pCamera = CreateShared<FlyCamera>(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight(), glm::vec3(0.0f, 1.5f, 2.0f));
	}

	void Player::Update(float _fDeltaTime)
	{
		// Update Camera
		m_pCamera->Inputs(BackEnd::GetGLFWwindow(), _fDeltaTime);
		m_pCamera->Update(m_weaponViewMatrix, 45.0f, 0.1f, 100.0f);

		// Update Inputs

	}

	void Player::FixedUpdate(float _fFixedDeltaTime)
	{

	}

	Shared<Camera> Player::GetCamera()
	{
		return m_pCamera;
	}

	void Player::SetWeaponViewMatrix(const glm::mat4& _weaponViewMatrix)
	{
		m_weaponViewMatrix = _weaponViewMatrix;
	}

}