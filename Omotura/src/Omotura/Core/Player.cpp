#include "Player.h"
#include "PlayerInput.h"

#include "PlayerStates/AimTransitionState.h"
#include "PlayerStates/AimAttackState.h"
#include "PlayerStates/AimIdleState.h"
#include "PlayerStates/AimWalkState.h"

#include "PlayerStates/AttackState.h"
#include "PlayerStates/DrawState.h"
#include "PlayerStates/IdleState.h"
#include "PlayerStates/ReloadState.h"
#include "PlayerStates/WalkState.h"

#include "../BackEnd/BackEnd.h"
#include "../Input/Input.h"
#include "../Input/KeyCodes.h"
#include "../Audio/Audio.h"
#include "../Asset/AssetManager.h"

namespace Omotura
{
	Player::Player()
		: m_fSpeed(constants::fPlayerSpeed),
		m_vFacingDir()
	{
		m_pCamera = CreateShared<Camera>(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight(), glm::vec3(0.0f, 0.0f, 0.0f));
		m_pCurrentWeapon = AssetManager::GetAsset<SkinnedModel>(hashID("AKS74U"));
		
		// Register callbacks
		PlayerInput::RegisterMoveCallback([this] (const Vector3& _vDir, const Quaternion& _qOrientation) {
			this->OnMove(_vDir, _qOrientation);
		});

		// Register animator
		m_pAnimator->Register();
	}

	void Player::Init()
	{
		// Animation states
		Shared<AimTransitionState> aimTransitionState = CreateShared<AimTransitionState>();
		Shared<AimAttackState> aimAttackState = CreateShared<AimAttackState>();
		Shared<AimIdleState> aimIdleState = CreateShared<AimIdleState>();
		Shared<AimWalkState> aimWalkState = CreateShared<AimWalkState>();
		
		Shared<AttackState> attackState = CreateShared<AttackState>();
		Shared<DrawState> drawState = CreateShared<DrawState>();
		Shared<IdleState> m_pIdleState = CreateShared<IdleState>();
		Shared<ReloadState> reloadState = CreateShared<ReloadState>();
		Shared<WalkState> m_pWalkState = CreateShared<WalkState>();

		// Draw Transition
		m_pAnimator->At(drawState, m_pIdleState, FuncPredicate([this]() {
			return m_pCurrentWeapon->CurrentAnimationFinished();
		}));
		
		// Idle Transitions
		m_pAnimator->At(m_pIdleState, aimTransitionState, FuncPredicate([this]() {
			return Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT);
		}));

		m_pAnimator->At(m_pIdleState, attackState, FuncPredicate([this]() {
			return Input::KeyPressed(OMOTURA_MOUSE_BUTTON_LEFT);
		}));
		
		m_pAnimator->At(m_pIdleState, reloadState, FuncPredicate([this]() {
			return Input::KeyPressed(OMOTURA_KEY_R);
		}));

		m_pAnimator->At(m_pIdleState, m_pWalkState, FuncPredicate([this]() {
			return this->m_vFacingDir != Vector3(0.0f);
		}));

		// Walk Transitions
		m_pAnimator->At(m_pWalkState, m_pIdleState, FuncPredicate([this]() {
			return this->m_vFacingDir == Vector3(0.0f);
		}));

		m_pAnimator->At(m_pWalkState, reloadState, FuncPredicate([this]() {
			return Input::KeyPressed(OMOTURA_KEY_R);
		}));

		m_pAnimator->At(m_pWalkState, attackState, FuncPredicate([this]() {
			return Input::KeyPressed(OMOTURA_MOUSE_BUTTON_LEFT);
		}));

		m_pAnimator->At(m_pWalkState, aimTransitionState, FuncPredicate([this]() {
			return Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT);
		}));

		// Aim Transitions
		m_pAnimator->At(aimTransitionState, m_pIdleState, FuncPredicate([this]() {
			return !Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT) && this->m_vFacingDir == Vector3(0.0f);
		}));

		m_pAnimator->At(aimTransitionState, m_pWalkState, FuncPredicate([this]() {
			return !Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT) && this->m_vFacingDir != Vector3(0.0f);
		}));

		m_pAnimator->At(aimTransitionState, aimIdleState, FuncPredicate([this]() {
			return Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT) && m_pCurrentWeapon->CurrentAnimationFinished();
		}));

		m_pAnimator->At(aimIdleState, aimTransitionState, FuncPredicate([this]() {
			return !Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT);
		}));

		m_pAnimator->At(aimIdleState, aimWalkState, FuncPredicate([this]() {
			return this->m_vFacingDir != Vector3(0.0f);
		}));

		m_pAnimator->At(aimWalkState, aimIdleState, FuncPredicate([this]() {
			return this->m_vFacingDir == Vector3(0.0f) || !Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT);
		}));

		m_pAnimator->At(aimWalkState, aimAttackState, FuncPredicate([this]() {
			return Input::KeyPressed(OMOTURA_MOUSE_BUTTON_LEFT);
		}));

		m_pAnimator->At(aimIdleState, aimAttackState, FuncPredicate([this]() {
			return Input::KeyPressed(OMOTURA_MOUSE_BUTTON_LEFT);
		}));

		m_pAnimator->At(aimAttackState, aimIdleState, FuncPredicate([this]() {
			return !Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT) || (this->m_vFacingDir == Vector3(0.0f) && m_pCurrentWeapon->CurrentAnimationFinished());
		}));

		m_pAnimator->At(aimAttackState, aimWalkState, FuncPredicate([this]() {
			return !Input::KeyDown(OMOTURA_MOUSE_BUTTON_RIGHT) || (this->m_vFacingDir != Vector3(0.0f) && m_pCurrentWeapon->CurrentAnimationFinished());
		}));

		// Attack Transitions
		m_pAnimator->At(attackState, m_pIdleState, FuncPredicate([this]() {
			return this->m_vFacingDir == Vector3(0.0f) && m_pCurrentWeapon->CurrentAnimationFinished();
		}));

		m_pAnimator->At(attackState, m_pWalkState, FuncPredicate([this]() {
			return this->m_vFacingDir != Vector3(0.0f) && m_pCurrentWeapon->CurrentAnimationFinished();
		}));

		// Reload Transitions
		m_pAnimator->At(reloadState, m_pIdleState, FuncPredicate([this]() {
			return this->m_vFacingDir == Vector3(0.0f) && m_pCurrentWeapon->CurrentAnimationFinished();
		}));

		m_pAnimator->At(reloadState, m_pWalkState, FuncPredicate([this]() {
			return this->m_vFacingDir != Vector3(0.0f) && m_pCurrentWeapon->CurrentAnimationFinished();
		}));

		m_pAnimator->SetDefaultState(drawState);
	}

	void Player::Update(float _fDeltaTime)
	{
		// Switch between weapons
		if (Input::KeyPressed(OMOTURA_KEY_1))
		{
			m_pCurrentWeapon->Hide();
			m_pCurrentWeapon = AssetManager::GetAsset<SkinnedModel>(hashID("AKS74U"));
			m_pCurrentWeapon->Show();
			m_pCurrentWeapon->ResetAnimation();
			m_pAnimator->ResetToDefault();
		}
		if (Input::KeyPressed(OMOTURA_KEY_2))
		{
			m_pCurrentWeapon->Hide();
			m_pCurrentWeapon = AssetManager::GetAsset<SkinnedModel>(hashID("Glock"));
			m_pCurrentWeapon->Show();
			m_pCurrentWeapon->ResetAnimation();
			m_pAnimator->ResetToDefault();
		}
		if (Input::KeyPressed(OMOTURA_KEY_3))
		{
			m_pCurrentWeapon->Hide();
			m_pCurrentWeapon = AssetManager::GetAsset<SkinnedModel>(hashID("Tokarev"));
			m_pCurrentWeapon->Show();
			m_pCurrentWeapon->ResetAnimation();
			m_pAnimator->ResetToDefault();
		}

		// Update Inputs
		HandleMovement(_fDeltaTime);

		// Update Camera
		m_pCamera->Follow(m_transform);
	}

	void Player::FixedUpdate(float _fFixedDeltaTime)
	{

	}

	void Player::HandleMovement(float _fDeltaTime)
	{
		// Update speed
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

		// Update model matrix
		glm::mat4 objectModel = glm::mat4(1.0f);
		glm::mat4 mTranslate = glm::translate(glm::vec3(m_transform.m_vWorldPosition.x, m_transform.m_vWorldPosition.y, m_transform.m_vWorldPosition.z));
		//glm::mat4 mRot = glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mScale = glm::scale(glm::vec3(0.0001f));
		objectModel = mTranslate * glm::mat4_cast(m_transform.m_quaternion.ToGLM()) * mScale* objectModel;
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

	Shared<SkinnedModel> Player::GetCurrentWeapon()
	{
		return m_pCurrentWeapon;
	}

	Transform Player::GetTransform()
	{
		return m_transform;
	}
}