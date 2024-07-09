#include "FlyCamera.h"

namespace Omotura
{
	FlyCamera::FlyCamera(int _iWidth, int _iHeight, glm::vec3 _vPosition)
		: Camera(_iWidth, _iHeight, _vPosition)
	{
		UpdateCameraAttributes();
	}

	void FlyCamera::ProcessKeyboard(CameraDirection _direction, float _fDeltaTime)
	{
		if (_direction == CameraDirection::FORWARD)
		{
			m_vPosition += m_fSpeed * m_vFront;
		}

		if (_direction == CameraDirection::BACKWARD)
		{
			m_vPosition -= m_fSpeed * m_vFront;
		}

		if (_direction == CameraDirection::RIGHT)
		{
			m_vPosition += m_fSpeed * m_vRight;
		}

		if (_direction == CameraDirection::LEFT)
		{
			m_vPosition -= m_fSpeed * m_vRight;
		}

		if (_direction == CameraDirection::UP)
		{
			m_vPosition += m_fSpeed * m_vUp;
		}

		if (_direction == CameraDirection::DOWN)
		{
			m_vPosition -= m_fSpeed * m_vUp;
		}
	}

	void FlyCamera::ProcessMouseMovement(GLFWwindow* _pWindow)
	{
		if (glfwGetMouseButton(_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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
		else if (glfwGetMouseButton(_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			glfwSetInputMode(_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_bFirstClick = true;
		}
	}

	void FlyCamera::UpdateCameraAttributes()
	{
		// Front (euler)
		//glm::vec3 vFront;
		//vFront.x = cos(glm::radians(m_fPitch)) * cos(glm::radians(m_fYaw));
		//vFront.y = sin(glm::radians(m_fPitch));
		//vFront.z = cos(glm::radians(m_fPitch)) * sin(glm::radians(m_fYaw));
		//m_vFront = glm::normalize(vFront);

		// Front (quaternions)
		m_vFront = glm::rotate(m_vFront, glm::radians(m_fPitch), m_vRight);
		m_vFront = glm::rotate(m_vFront, glm::radians(-m_fYaw), m_vUp);

		// Right
		m_vRight = glm::normalize(glm::cross(m_vFront, m_vWorldUp));
		m_vUp = glm::normalize(glm::cross(m_vRight, m_vFront));
	}
}