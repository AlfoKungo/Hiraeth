#include "hrth_pch.h"
#include "keyboard.h"

namespace hiraeth {
	namespace input {
		Keyboard::Keyboard()
		{
			for (bool& m_Key : m_Keys)
				m_Key = false;
			for (bool& i : m_KeysClicked)
				i = false;
			for (bool& m_MouseButton : m_MouseButtons)
				m_MouseButton = false;
			//initControls();
		}

		bool Keyboard::isKeyPressed(KeyCode keycode) const
		{
			if (keycode >= GLFW_KEY_LAST)
				return false;

			return m_Keys[keycode];
		}

		bool Keyboard::isKeyClicked(KeyCode keycode)
		{
			if (keycode >= GLFW_KEY_LAST)
				return false;

			if (m_KeysClicked[keycode])
			{
				m_KeysClicked[keycode] = false;
				return true;
			}
			return false;
		}

		bool Keyboard::isMouseButtonPressed(unsigned int button) const
		{
			if (button >= GLFW_MOUSE_BUTTON_LAST)
				return false;

			return m_MouseButtons[button];
		}

		void Keyboard::getMousePosition(double& x, double& y) const
		{
			x = mx;
			y = my;
		}
		maths::vec2 Keyboard::getMousePosition() const
		{
			return maths::vec2{ float(mx), float(my) };
		}

		void Keyboard::setTyping(KeyboardEvent* keyboard_event, bool is_typing)
		{
			if (is_typing)
				m_TypingKeyboardEvent = keyboard_event;
			else
				m_TypingKeyboardEvent = nullptr;
		}

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto kb = static_cast<Keyboard*>(glfwGetWindowUserPointer(window));
			if (key == GLFW_KEY_RIGHT_CONTROL)
				key = GLFW_KEY_LEFT_CONTROL;
			if (key == GLFW_KEY_RIGHT_ALT)
				key = GLFW_KEY_LEFT_ALT;
			kb->m_Keys[key] = action;
			kb->clicked(key, action);
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			auto kb = static_cast<Keyboard*>(glfwGetWindowUserPointer(window));
			kb->m_MouseButtons[button] = action != GLFW_RELEASE;
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
				for (auto const& event_window : kb->m_MouseEventMap)
				{
					if (event_window->is_window_contains(maths::vec2{ float(kb->pmx), float(kb->pmy) }))
					{
						event_window->leftButtonClicked(kb->pmx, kb->pmy);
						return;
					}
				}
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
				for (auto const& event_window : kb->m_MouseEventMap)
				{
					event_window->leftButtonReleased(kb->pmx, kb->pmy);
				}
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
				for (auto const& event_window : kb->m_MouseEventMap)
				{
					event_window->rightButtonClicked(kb->pmx, kb->pmy);
				}

		}
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			auto kb = static_cast<Keyboard*>(glfwGetWindowUserPointer(window));
			kb->pmx = kb->mx;
			kb->pmy = kb->my;
			kb->mx = xpos;
			kb->my = ypos;
			//if (kb->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
				for (auto const& ref_window : kb->m_MouseEventMap)
				{
					if (ref_window->mouseMove(kb->pmx, kb->pmy, xpos, ypos))
						return;
				}

		}

		void char_callback(GLFWwindow* window, unsigned int codepoint)
		{
			auto kb = static_cast<Keyboard*>(glfwGetWindowUserPointer(window));
			kb->char_input(codepoint);
		}
	}
}
