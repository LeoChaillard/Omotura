#pragma once

#include "../Renderer/ShaderClass.h"
#include "../Common/Common.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Omotura
{
	namespace constants
	{
		constexpr float fCameraYaw = 0.0f;
		constexpr float fCameraPitch = 0.0f;
		constexpr float fCameraSpeed = 0.08f;
		constexpr float fCameraSensitivity = 0.08f;
		constexpr float fCameraFOV = 45.0f;
	}

	enum class CameraDirection
	{
		NONE,
		FORWARD,
		BACKWARD,
		RIGHT,
		LEFT,
		UP,
		DOWN
	};

	class Camera
	{
	public:
		// Camera attributes
		glm::vec3 m_vPosition;
		glm::vec3 m_vFront = glm::vec3(0.0f, 0.0, -1.0f);
		glm::vec3 m_vWorldUp = glm::vec3(0.0f, 1.0, 0.0f);
		glm::vec3 m_vUp = glm::vec3(0.0f, 1.0, 0.0f);
		glm::vec3 m_vRight = glm::vec3(1.0f, 0.0, 0.0f);;

	protected:
		glm::mat4 m_mCameraMatrix = glm::mat4(1.0f);
		int m_iViewportWidth;
		int m_iViewportHeight;
		bool m_bFirstClick = true;

		// Euler angles
		float m_fYaw;
		float m_fPitch;

		// Camera settings
		float m_fSpeed;
		float m_fSensitivity;
		float m_fFOV;

	public:
		Camera(int _iWidth, int _iHeight, glm::vec3 _vPosition);

		void Update(glm::mat4 _weaponViewMatrix, float _fFOV, float _fNearPlane, float _fFarPlane);
		void UploadMatrix(Shader& _shader, const char* _pUniform);
		void Inputs(GLFWwindow* _pWindow, float _fDeltaTime);

	protected:
		virtual void ProcessKeyboard(CameraDirection _direction, float _fDeltaTime) = 0;
		virtual void ProcessMouseMovement(GLFWwindow* _pWindow) = 0;
	};
}
