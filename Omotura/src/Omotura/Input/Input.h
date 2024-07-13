#pragma once

#include "../BackEnd/BackEnd.h"
#include "../Common/Common.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Omotura
{
	namespace constants
	{
		constexpr int iKeyboardMinCode = 32;
		constexpr int iKeyboardNumCode = 349;

		constexpr int iMouseMinCode = 0;
		constexpr int iMouseNumCode = 12;
	}

	class Input
	{
	private:
		// Window
		static GLFWwindow* m_pWindow;

		// Mouse Position
		static double m_lfMouseXOffset;
		static double m_lfMouseYOffset;
		static double m_lfMouseX;
		static double m_lfMouseY;

		// Mouse Orientation
		static double m_lfMouseLastX;
		static double m_lfMouseLastY;
		static bool m_bFirstClick;
		static float m_fPitch;
		static float m_fYaw;
		static glm::vec3 m_vFront;
		static glm::vec3 m_vRight;
		static glm::vec3 m_vUp;

		// Mouse Down/Pressed
		static bool m_aMouseDown[constants::iMouseNumCode];
		static bool m_aMousePressed[constants::iMouseNumCode];
		static bool m_aMouseDownLastFrame[constants::iMouseNumCode];

		// Keyboard
		static bool m_aKeyDown[constants::iKeyboardNumCode];
		static bool m_aKeyPressed[constants::iKeyboardNumCode];
		static bool m_aKeyDownLastFrame[constants::iKeyboardNumCode];

		// Scroll
		static double m_lfScrollYOffset;
		static bool m_lfScrollDown;
		static bool m_lfScrollUp;

	public:
		static void Init();
		static void Update();

		// Mouse
		static double GetMouseOffsetX();
		static double GetMouseOffsetY();
		static double GetMouseX();
		static double GetMouseY();
		static void DisableCursor();
		static void ShowCursor();

		// Keyboard
		static bool KeyPressed(uint16_t _keyCode);
		static bool KeyDown(uint16_t _keyCode);

		// Scroll
		static double GetScrollYOffset();
		static bool ScrollDown();
		static bool ScrollUp();

	private:
		static void ScrollCallback(GLFWwindow* _pWindow, double _lfOffsetX, double _lfOffsetY);
		static void ResetScroll();
		static void HandleInputs();
	};
}