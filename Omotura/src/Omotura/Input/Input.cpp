#include "Input.h"
#include "KeyCodes.h"

#include <GLFW/glfw3.h>

namespace Omotura
{
	// Window
	GLFWwindow* Input::m_pWindow;

	// Mouse Position
	double Input::m_lfMouseXOffset;
	double Input::m_lfMouseYOffset;
	double Input::m_lfMouseX;
	double Input::m_lfMouseY;

	// Mouse Down/Pressed
	bool Input::m_aMouseDown[constants::iMouseNumCode];
	bool Input::m_aMousePressed[constants::iMouseNumCode];
	bool Input::m_aMouseDownLastFrame[constants::iMouseNumCode];

	// Keyboard
	bool Input::m_aKeyDown[constants::iKeyboardNumCode];
	bool Input::m_aKeyPressed[constants::iKeyboardNumCode];
	bool Input::m_aKeyDownLastFrame[constants::iKeyboardNumCode];

	// Scroll
	double Input::m_lfScrollYOffset;
	bool Input::m_lfScrollDown;
	bool Input::m_lfScrollUp;

	void Input::Init()
	{
		// Window
		m_pWindow = BackEnd::GetGLFWwindow();
		glfwSetScrollCallback(m_pWindow, ScrollCallback);

		// Mouse position
		double lfX, lfY;
		glfwGetCursorPos(m_pWindow, &lfX, &lfY);
		m_lfMouseXOffset = lfX;
		m_lfMouseYOffset = lfY;
		m_lfMouseX = lfX;
		m_lfMouseY = lfY;
	}

	void Input::Update()
	{
		HandleInputs();
	}

	void Input::HandleInputs()
	{
		// Keyboard
		int iKeyMinCode = constants::iKeyboardMinCode;
		int iKeyNumCode = constants::iKeyboardNumCode;
		for (int i = iKeyMinCode; i < iKeyNumCode; i++)
		{
			// Key Down
			if (glfwGetKey(m_pWindow, i) == GLFW_PRESS)
			{
				m_aKeyDown[i] = true;
			}
			else if (glfwGetKey(m_pWindow, i) == GLFW_RELEASE)
			{
				m_aKeyDown[i] = false;
			}

			// Key Presed
			if (m_aKeyDown[i] && !m_aKeyDownLastFrame[i])
			{
				m_aKeyPressed[i] = true;
			}
			else
			{
				m_aKeyPressed[i] = false;
			}
			m_aKeyDownLastFrame[i] = m_aKeyDown[i];
		}

		// Mouse Position
		double lfX, lfY;
		glfwGetCursorPos(m_pWindow, &lfX, &lfY);
		m_lfMouseXOffset = lfX - m_lfMouseX;
		m_lfMouseYOffset = m_lfMouseY - lfY;
		m_lfMouseX = lfX;
		m_lfMouseY = lfY;

		// Mouse Down/Pressed
		int iMouseMinCode = constants::iMouseMinCode;
		int iMouseNumCode = constants::iMouseNumCode;
		for (int i = iMouseMinCode; i < iMouseNumCode; i++)
		{
			// Mouse Down
			if (glfwGetMouseButton(m_pWindow, i) == GLFW_PRESS)
			{
				m_aMouseDown[i] = true;
			}
			else if (glfwGetMouseButton(m_pWindow, i) == GLFW_RELEASE)
			{
				m_aMouseDown[i] = false;
			}

			// Pressed Presed
			if (m_aMouseDown[i] && !m_aMouseDownLastFrame[i])
			{
				m_aMousePressed[i] = true;
			}
			else
			{
				m_aMousePressed[i] = false;
			}
			m_aMouseDownLastFrame[i] = m_aMouseDown[i];
		}

		// Scroll
		if (m_lfScrollYOffset < 0)
		{
			m_lfScrollDown = true;
		}
		else if (m_lfScrollYOffset > 0)
		{
			m_lfScrollUp = true;
		}
		ResetScroll();
	}

	bool Input::KeyPressed(uint16_t _keyCode)
	{
		// Keyboard
		int iKeyMinCode = constants::iKeyboardMinCode;
		int iKeyNumCode = constants::iKeyboardNumCode;
		if (iKeyMinCode <= _keyCode && _keyCode < iKeyNumCode)
		{
			return m_aKeyPressed[_keyCode];
		}

		// Mouse
		int iMouseMinCode = constants::iMouseMinCode;
		int iMouseNumCode = constants::iMouseNumCode;
		if (iMouseMinCode <= _keyCode && _keyCode < iMouseNumCode)
		{
			return m_aMousePressed[_keyCode];
		}

		return false;
	}

	bool Input::KeyDown(uint16_t _keyCode)
	{
		// Keyboard
		int iKeyMinCode = constants::iKeyboardMinCode;
		int iKeyNumCode = constants::iKeyboardNumCode;
		if (iKeyMinCode <= _keyCode && _keyCode < iKeyNumCode)
		{
			return m_aKeyDown[_keyCode];
		}

		// Mouse
		int iMouseMinCode = constants::iMouseMinCode;
		int iMouseNumCode = constants::iMouseNumCode;
		if (iMouseMinCode <= _keyCode && _keyCode < iMouseNumCode)
		{
			return m_aMouseDown[_keyCode];
		}

		return false;
	}

	double Input::GetMouseOffsetX()
	{
		return m_lfMouseXOffset;
	}

	double Input::GetMouseOffsetY()
	{
		return m_lfMouseYOffset;
	}
			
	double Input::GetMouseX()
	{
		return m_lfMouseX;
	}
			
	double Input::GetMouseY()
	{
		return m_lfMouseY;
	}

	void Input::DisableCursor()
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Input::ShowCursor()
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
			
	double Input::GetScrollYOffset()
	{
		return m_lfScrollYOffset;
	}

	bool Input::ScrollDown()
	{
		return m_lfScrollDown;
	}

	bool Input::ScrollUp()
	{
		return m_lfScrollUp;
	}

	void Input::ScrollCallback(GLFWwindow* _pWindow, double _lfOffsetX, double _lfOffsetY)
	{
		m_lfScrollYOffset = _lfOffsetY;
	}

	void Input::ResetScroll()
	{
		m_lfScrollDown = false;
		m_lfScrollUp = false;
		m_lfScrollYOffset = 0.0;
	}
}