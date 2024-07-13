#pragma once

#include "Transform.h"

#include "../Common/Common.h"

#include <glm/glm.hpp>

namespace Omotura
{
	class PlayerInput
	{
	public:
		// Mouse Orientation
		static float m_fPitch;
		static float m_fYaw;
		static glm::vec3 m_vFront;
		static glm::vec3 m_vRight;
		static glm::vec3 m_vUp;

		// Movement direction
		static Vector3 m_vMoveDirection;

		// Quaternions
		static glm::quat m_qOrientation;

	public:
		static void Update();

		// Callbacks
		static void RegisterMoveCallback(OnMoveCallback _callback);

	private:
		static void ExecuteCallbacks();
		static void ProcessInputs();
		static void ProcessMouse();
		static void ProcessKeyboard();

		//TODO - KeyBindings: void RegisterDefaultSetting()
	};
}