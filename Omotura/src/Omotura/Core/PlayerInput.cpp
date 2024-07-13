#include "PlayerInput.h"

#include "../Input/Input.h"
#include "../Input/KeyCodes.h"

namespace Omotura
{
	namespace constants
	{
		constexpr float fMouseSensitivity = 0.05f;
	}

	// Mouse Orientation
	float PlayerInput::m_fPitch;
	float PlayerInput::m_fYaw;
	glm::vec3 PlayerInput::m_vFront(0.0f, 0.0f, -1.0f);
	glm::vec3 PlayerInput::m_vRight(1.0f, 0.0f, 0.0f);
	glm::vec3 PlayerInput::m_vUp(0.0f, 1.0f, 0.0f);

	// Movement direction
	Vector3 PlayerInput::m_vMoveDirection;

	// Quaternions
	glm::quat PlayerInput::m_qOrientation;

	// Callbacks
	OnMoveCallback m_onMoveCallback;

	void PlayerInput::Update()
	{
		ProcessInputs();
		ExecuteCallbacks();
	}

	void PlayerInput::ProcessInputs()
	{
		ProcessMouse();
		ProcessKeyboard();
	}

	void PlayerInput::ProcessMouse()
	{
		// Check Mouse Inputs
		Input::DisableCursor();

		// Angles from mouse movement
		m_fYaw += (constants::fMouseSensitivity* (float)Input::GetMouseOffsetX());		
		m_fYaw = fmod(m_fYaw, 360.0f);
		m_fPitch += (constants::fMouseSensitivity * (float)Input::GetMouseOffsetY());
		m_fPitch = fmod(m_fPitch, 360.0f);

		// Constrain pitch to avoid screen flipping
		if (m_fPitch > 89.0f)
		{
			m_fPitch = 89.0f;
		}
		else if (m_fPitch < -89.0f)
		{
			m_fPitch = -89.0f;
		}

		// Orientation
		m_qOrientation = glm::angleAxis(glm::radians(-m_fYaw), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::angleAxis(glm::radians(m_fPitch), glm::vec3(1.0f, 0.0f, 0.0f));
		m_qOrientation = glm::normalize(m_qOrientation);

		// Update axis
		m_vFront = glm::vec3(0.0f, 0.0f, -1.0f) * glm::inverse(m_qOrientation);
		m_vRight = glm::normalize(glm::cross(m_vFront, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_vUp = glm::normalize(glm::cross(m_vRight, m_vFront));
	}

	void PlayerInput::ProcessKeyboard()
	{
		// Check Keyboard Inputs
		Vector3 vMoveDirection(0.0f);
		bool bPressed = false;

		if (Input::KeyDown(OMOTURA_KEY_W))
		{
			vMoveDirection += Vector3(m_vFront.x, m_vFront.y, m_vFront.z); // Facing -z
			bPressed = true;
		}
		if (Input::KeyDown(OMOTURA_KEY_S))
		{
			vMoveDirection -= Vector3(m_vFront.x, m_vFront.y, m_vFront.z);
			bPressed = true;
		}
		if (Input::KeyDown(OMOTURA_KEY_D))
		{
			vMoveDirection += Vector3(m_vRight.x, m_vRight.y, m_vRight.z);
			bPressed = true;
		}
		if (Input::KeyDown(OMOTURA_KEY_A))
		{
			vMoveDirection -= Vector3(m_vRight.x, m_vRight.y, m_vRight.z);
			bPressed = true;
		}

		m_vMoveDirection = bPressed ? vMoveDirection : Vector3(0.0f);
	}

	void PlayerInput::ExecuteCallbacks()
	{
		// Execute
		if (m_onMoveCallback)
		{
			m_onMoveCallback(m_vMoveDirection, Quaternion(m_qOrientation.w, Vector3(m_qOrientation.x, m_qOrientation.y, m_qOrientation.z)));
		}
	}

	void PlayerInput::RegisterMoveCallback(OnMoveCallback _callback)
	{
		m_onMoveCallback = _callback;
	}
}