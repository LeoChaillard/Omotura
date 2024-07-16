#include "Camera.h"

#include "../BackEnd/BackEnd.h"

#include "Game.h"

namespace Omotura
{
	namespace constants
	{
		constexpr float fCameraFOV = 45.0f;
		constexpr float fCameraNearPlane = 0.1f;
		constexpr float fCameraFarPlane = 100.0f;
	}

	Camera::Camera(int _iViewportWidth, int _iViewportHeight, glm::vec3 _vPosition)
		: m_transform(),
		m_iViewportWidth(_iViewportWidth),
		m_iViewportHeight(_iViewportHeight)
	{		
		m_transform.m_vWorldPosition = Vector3(0.0f, 2.0f, 10.0f);
		m_transform.m_vLocalPosition = Vector3(0.0f, 0.85f, 0.1f);
		m_transform.m_quaternion = Quaternion(1.0f, Vector3(0.0f));
	}

	void Camera::Follow(const Transform& _transform)
	{
		// Update camera position
		m_transform.m_quaternion = _transform.m_quaternion;
		m_transform.m_vWorldPosition = _transform.m_vWorldPosition + m_transform.m_quaternion * m_transform.m_vLocalPosition;
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::mat4_cast(glm::inverse(m_transform.m_quaternion.ToGLM())) * glm::translate(-m_transform.m_vWorldPosition.ToGLM());
	}

	glm::mat4 Camera::GetPerspectiveMatrix()
	{
		return glm::perspective(glm::radians(constants::fCameraFOV), (float)m_iViewportWidth / m_iViewportHeight, constants::fCameraNearPlane, constants::fCameraFarPlane);
	}

	glm::mat4 Camera::GetInverseViewMatrix()
	{
		return glm::translate(m_transform.m_vWorldPosition.ToGLM()) * glm::mat4_cast(m_transform.m_quaternion.ToGLM());
	}

	glm::mat4 Camera::GetOrientation()
	{
		return glm::mat4_cast(glm::inverse(m_transform.m_quaternion.ToGLM()));
	}

	const Transform& Camera::GetTransform() const
	{
		return m_transform;
	}
}