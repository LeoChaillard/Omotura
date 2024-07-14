#include "Player.h"
#include "PlayerInput.h"

#include "../BackEnd/BackEnd.h"
#include "../Input/Input.h"
#include "../Input/KeyCodes.h"
#include "../Audio/Audio.h"
#include "../Asset/AssetManager.h"

namespace Omotura
{
	Player::Player()
		: m_transform(),
		m_fSpeed(constants::fPlayerSpeed),
		m_vFacingDir()
	{
		m_pCamera = CreateShared<Camera>(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight(), glm::vec3(0.0f, 0.0f, 0.0f));
		m_pCurrentWeapon = AssetManager::GetAsset<SkinnedModel>(hashID("Glock"));
		
		// Register callbacks
		PlayerInput::RegisterMoveCallback([this] (const Vector3& _vDir, const Quaternion& _qOrientation) {
			this->OnMove(_vDir, _qOrientation);
		});
	}

	void Player::Update(float _fDeltaTime)
	{
		// Update Inputs
		HandleMovement(_fDeltaTime);

		// Testing sound and animations
		if (Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT))
		{
			glm::mat4 objectModel = glm::mat4(1.0f);
			glm::mat4 mTranslate = glm::translate(glm::vec3(m_transform.m_vWorldPosition.x, m_transform.m_vWorldPosition.y, m_transform.m_vWorldPosition.z));
			glm::mat4 mScale = glm::scale(glm::vec3(0.0001f));
			glm::mat4 mRot = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			objectModel = mTranslate * glm::mat4_cast(m_transform.m_quaternion.ToGLM()) * mRot * mScale * objectModel;
			m_pCurrentWeapon->SetModelMatrix(objectModel);

			m_pCurrentWeapon->SetAnimation("Glock_ADS_In", false);

			if (Input::KeyPressed(OMOTURA_MOUSE_BUTTON_LEFT))
			{
				//m_pCurrentWeapon->SetAnimation("Glock_Fire0", false);
				Audio::Play("Glock_Fire0", 0.1f, false);
			}

			if (!Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT))
			{
				glm::mat4 objectModel = glm::mat4(1.0f);
				glm::mat4 mTranslate = glm::translate(glm::vec3(m_transform.m_vWorldPosition.x, m_transform.m_vWorldPosition.y, m_transform.m_vWorldPosition.z));
				glm::mat4 mRot = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 mScale = glm::scale(glm::vec3(0.0001f));
				objectModel = mTranslate * glm::mat4_cast(m_transform.m_quaternion.ToGLM()) * mScale * objectModel;
				m_pCurrentWeapon->SetModelMatrix(objectModel);

				m_pCurrentWeapon->SetAnimation("Glock_ADS_Out", false);
			}
		}


		// Update Camera
		m_pCamera->Follow(m_transform);
	}

	void Player::FixedUpdate(float _fFixedDeltaTime)
	{

	}

	void Player::HandleMovement(float _fDeltaTime)
	{
		// Update speed
		if (m_vFacingDir == Vector3(0.0f))
		{
			m_pCurrentWeapon->SetAnimation("Glock_Idle");
		}
		else
		{
			m_pCurrentWeapon->SetAnimation("Glock_Walk");
		}

		if (m_vFacingDir.z != 0 && m_vFacingDir.x != 0)
		{
			m_fSpeed = constants::fPlayerSpeed / 3.0f;
		}
		else
		{
			m_fSpeed = constants::fPlayerSpeed;
		}

		// Update position
		m_transform.m_vWorldPosition += m_vFacingDir * m_fSpeed * _fDeltaTime;
		m_transform.m_vWorldPosition.y = 2.0f;

		// Update weapon model matrix
		glm::mat4 objectModel = glm::mat4(1.0f);
		glm::mat4 mTranslate = glm::translate(glm::vec3(m_transform.m_vWorldPosition.x, m_transform.m_vWorldPosition.y, m_transform.m_vWorldPosition.z));
		glm::mat4 mScale = glm::scale(glm::vec3(0.0001f));
		//glm::mat4 mRot = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mRot(1.0f);
		objectModel = mTranslate * glm::mat4_cast(m_transform.m_quaternion.ToGLM()) * mRot * mScale * objectModel;
		m_pCurrentWeapon->SetModelMatrix(objectModel);			
	}

	void Player::OnMove(const Vector3& _vDir, const Quaternion& _qOrientation)
	{
		m_vFacingDir = _vDir;
		m_transform.m_quaternion = _qOrientation;
	}

	Shared<Camera> Player::GetCamera()
	{
		return m_pCamera;
	}
}