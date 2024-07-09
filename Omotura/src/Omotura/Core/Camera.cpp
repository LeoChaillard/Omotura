#include "Camera.h"

#include "../BackEnd/BackEnd.h"

namespace Omotura
{
	Camera::Camera(int _iViewportWidth, int _iViewportHeight, glm::vec3 _vPosition)
		: m_vPosition(_vPosition),
		m_iViewportWidth(_iViewportWidth),
		m_iViewportHeight(_iViewportHeight),
		m_fYaw(constants::fCameraYaw),
		m_fPitch(constants::fCameraPitch),
		m_fSpeed(constants::fCameraSpeed),
		m_fSensitivity(constants::fCameraSensitivity),
		m_fFOV(constants::fCameraFOV)		
	{		
	}

	void Camera::Update(glm::mat4 _weaponViewMatrix, float _fFOV, float _fNearPlane, float _fFarPlane)
	{
		glm::mat4 mView = glm::mat4(1.0f);
		glm::mat4 mProjection = glm::mat4(1.0f);

		mView = glm::lookAt(m_vPosition, m_vPosition + m_vFront, m_vUp);
		mProjection = glm::perspective(glm::radians(_fFOV), (float)m_iViewportWidth / m_iViewportHeight, _fNearPlane, _fFarPlane);

		m_mCameraMatrix = mProjection * glm::mat4(glm::mat3(_weaponViewMatrix)) * mView;
	}

	void Camera::UploadMatrix(Shader& _shader, const char* _pUniform)
	{
		_shader.SetMatrixFloat4(_pUniform, m_mCameraMatrix);
	}

	void Camera::Inputs(GLFWwindow* _pWindow, float _fDeltaTime)
	{
		// Keyboard
		if (glfwGetKey(_pWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			ProcessKeyboard(CameraDirection::FORWARD, _fDeltaTime);
		}

		if (glfwGetKey(_pWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			ProcessKeyboard(CameraDirection::BACKWARD, _fDeltaTime);
		}

		if (glfwGetKey(_pWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			ProcessKeyboard(CameraDirection::RIGHT, _fDeltaTime);
		}

		if (glfwGetKey(_pWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			ProcessKeyboard(CameraDirection::LEFT, _fDeltaTime);
		}

		if (glfwGetKey(_pWindow, GLFW_KEY_E) == GLFW_PRESS)
		{
			ProcessKeyboard(CameraDirection::UP, _fDeltaTime);
		}

		if (glfwGetKey(_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
		{
			ProcessKeyboard(CameraDirection::DOWN, _fDeltaTime);
		}

		// Mouse
		ProcessMouseMovement(_pWindow);
	}
}