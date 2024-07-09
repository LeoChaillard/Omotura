#pragma once

#include "Camera.h"

namespace Omotura
{
	class FPSCamera : public Camera
	{
	private:
		float m_fLastX;
		float m_fLastY;

	public:
		FPSCamera(int _iWidth, int _iHeight, glm::vec3 _vPosition);

	protected:
		virtual void ProcessKeyboard(CameraDirection _direction, float _fDeltaTime) override;
		virtual void ProcessMouseMovement(GLFWwindow* _pWindow) override;

	private:
		void UpdateCameraAttributes();
	};
}
