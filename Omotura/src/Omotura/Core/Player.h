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
	public:
		/*******DEBUG********/
		bool m_bFly = false;

	private:
		Shared<Camera> m_pCamera;
		Shared<SkinnedModel> m_pCurrentWeapon;

		// Movement
		float m_fMaxSpeed;
		float m_fSpeed;
		Vector3 m_vFacingDir;

	public:
		Player();
		~Player() = default;

		void Init();
		void Update(float _fDeltaTime);
		void FixedUpdate();

		Shared<Camera> GetCamera();
		Shared<SkinnedModel> GetCurrentWeapon();
		Transform GetTransform();

		/*******DEBUG********/
		void MultiplySpeed(float _fFactor);
		void SetDefaultSpeed();
		Shared<Animator> GetAnimator();

	private:		
		void OnMove(const Vector3& _vDir, const Quaternion& _qOrientation);
		void HandleMovement(float _fDeltaTime);
	};
}