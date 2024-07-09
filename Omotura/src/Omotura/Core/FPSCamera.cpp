#include "FPSCamera.h"

namespace Omotura
{
	namespace constants
	{
		constexpr float fFPSCameraSpeed = 5.0f;
		constexpr float fFPSCameraSensitivity = 0.05f;
	}

	FPSCamera::FPSCamera(int _iWidth, int _iHeight, glm::vec3 _vPosition)
		: Camera(_iWidth, _iHeight, _vPosition)
	{
		m_fSpeed = constants::fFPSCameraSpeed;
		m_fSensitivity = constants::fFPSCameraSensitivity;
		UpdateCameraAttributes();
	}

	void FPSCamera::ProcessKeyboard(CameraDirection _direction, float _fDeltaTime)
	{
		float fVelocity = m_fSpeed * _fDeltaTime;

		if (_direction == CameraDirection::FORWARD)
		{
			m_vPosition += fVelocity * m_vFront;
		}

		if (_direction == CameraDirection::BACKWARD)
		{
			m_vPosition -= fVelocity * m_vFront;
		}

		if (_direction == CameraDirection::RIGHT)
		{
			m_vPosition += fVelocity * m_vRight;
		}

		if (_direction == CameraDirection::LEFT)
		{
			m_vPosition -= fVelocity * m_vRight;
		}

		m_vPosition.y = 1.5f;
	}

	void FPSCamera::ProcessMouseMovement(GLFWwindow* _pWindow)
	{
		glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Retrieving mouse coordinates
		double lfMouseX, lfMouseY;
		glfwGetCursorPos(_pWindow, &lfMouseX, &lfMouseY);

		if (m_bFirstClick)
		{
			m_fLastX = lfMouseX;
			m_fLastY = lfMouseY;
			m_bFirstClick = false;
		}

		float fXOffset = lfMouseX - m_fLastX;
		float fYOffset = m_fLastY - lfMouseY;
		m_fLastX = lfMouseX;
		m_fLastY = lfMouseY;

		m_fYaw = (m_fSensitivity * fXOffset);
		m_fPitch = (m_fSensitivity * fYOffset);

		// Constrain pitch to avoid screen flipping
		if (m_fPitch > 89.0f)
		{
			m_fPitch = 89.0f;
		}
		else if (m_fPitch < -89.0f)
		{
			m_fPitch = -89.0f;
		}

		// Update atttributes
		UpdateCameraAttributes();
	}

	void FPSCamera::UpdateCameraAttributes()
	{
		// Front
		m_vFront = glm::rotate(m_vFront, glm::radians(m_fPitch), m_vRight);
		m_vFront = glm::rotate(m_vFront, glm::radians(-m_fYaw), m_vUp);

		// Right
		m_vRight = glm::normalize(glm::cross(m_vFront, m_vWorldUp));
		m_vUp = glm::normalize(glm::cross(m_vRight, m_vFront));
	}
}