#include "keyboard.h"

namespace hiraeth {
	namespace input {
		Keyboard::Keyboard()
		{

			for (int i = 0; i < GLFW_KEY_LAST; ++i)
				keys_map.insert(std::pair<unsigned int, KeyboardEvent*>(i, nullptr));
			for (int i = 0; i < GLFW_KEY_LAST; ++i)
				m_Keys[i] = false;
			for (int i = 0; i < GLFW_KEY_LAST; ++i)
				m_KeysClicked[i] = false;
			for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i)
				m_MouseButtons[i] = false;
			initControls();
		}

		bool Keyboard::isKeyPressed(unsigned int keycode) const
		{
			if (keycode >= GLFW_KEY_LAST)
				return false;

			return m_Keys[keycode];
		}

		bool Keyboard::isControlPressed(Controls control)
		{
			return m_Keys[m_Controls[control]];
		}

		bool Keyboard::isKeyClicked(unsigned int keycode)
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

		bool Keyboard::isControlClicked(Controls control)
		{
			unsigned int key = m_Controls[control];
			if (m_KeysClicked[key])
			{
				m_KeysClicked[key] = false;
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
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Keyboard* kb = (Keyboard*)glfwGetWindowUserPointer(window);
				if (key == GLFW_KEY_RIGHT_CONTROL)
					key = GLFW_KEY_LEFT_CONTROL;
				if (key == GLFW_KEY_RIGHT_ALT)
					key = GLFW_KEY_LEFT_ALT;
			kb->m_Keys[key] = action;
			kb->clicked(key, action);
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Keyboard* kb = (Keyboard*)glfwGetWindowUserPointer(window);
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
			Keyboard* kb = (Keyboard*)glfwGetWindowUserPointer(window);
			kb->pmx = kb->mx;
			kb->pmy = kb->my;
			kb->mx = xpos;
			kb->my = ypos;
			//if (kb->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
				for (auto const& window : kb->m_MouseEventMap)
				{
					if (window->mouseMove(kb->pmx, kb->pmy, xpos, ypos))
						return;
				}

		}

		void Keyboard::initControls()
		{
			setKeyControl(up);
			setKeyControl(down);
			setKeyControl(left);
			setKeyControl(right);
			setKeyControl(jump);
			setKeyControl(attack);
			setKeyControl(escape);
			setKeyControl(stats);
			setKeyControl(skills);
			setKeyControl(quests);
			setKeyControl(equip);
			setKeyControl(inventory);
			setKeyControl(pick_up);
		}

		void Keyboard::setKeyControl(Controls control)
		{
			setKeyControl(control, control);
		}

		void Keyboard::setKeyControl(Controls control, unsigned int key)
		{
			m_Controls[control] = key;
			m_KeysControls[key] = control;

		}
	}
}
