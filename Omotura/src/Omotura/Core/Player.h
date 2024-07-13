#pragma once

#include "Transform.h"

#include "../Renderer/SkinnedModel.h"

namespace Omotura
{
	namespace constants
	{
		constexpr float fPlayerSpeed = 20.0f;
	}

	class Player
	{
	private:
		Transform m_transform;
		Shared<Camera> m_pCamera;
		Shared<SkinnedModel> m_pCurrentWeapon;

		// Movement
		float m_fSpeed;
		Vector3 m_vFacingDir;

	public:
		Player();
		~Player() = default;

		void Update(float _fDeltaTime);
		void FixedUpdate(float _fFixedDeltaTime);

		Shared<Camera> GetCamera();

	private:		
		void OnMove(const Vector3& _vDir, const Quaternion& _qOrientation);
		void HandleMovement(float _fDeltaTime);
	};
}