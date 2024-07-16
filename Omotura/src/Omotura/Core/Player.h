#pragma once

#include "Transform.h"
#include "GameObject.h"

#include "../Renderer/SkinnedModel.h"

namespace Omotura
{
	namespace constants
	{
		constexpr float fPlayerSpeed = 20.0f;
	}

	class Player : public GameObject
	{
	private:
		Shared<Camera> m_pCamera;
		Shared<SkinnedModel> m_pCurrentWeapon;

		// Movement
		float m_fSpeed;
		Vector3 m_vFacingDir;

	public:
		Player();
		~Player() = default;

		void Init();
		void Update(float _fDeltaTime);
		void FixedUpdate(float _fFixedDeltaTime);

		Shared<Camera> GetCamera();
		Shared<SkinnedModel> GetCurrentWeapon();
		Transform GetTransform();

	private:		
		void OnMove(const Vector3& _vDir, const Quaternion& _qOrientation);
		void HandleMovement(float _fDeltaTime);
	};
}