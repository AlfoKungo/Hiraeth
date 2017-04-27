#include "keyboard.h"

namespace hiraeth {
	namespace input {

		Keyboard::Keyboard()
		{

			for (int i = 0; i < GLFW_KEY_LAST; i++)
				keys_map.insert(std::pair<unsigned int, KeyboardEvent*>(i, nullptr));
			for (int i = 0; i < GLFW_KEY_LAST; i++)
				m_Keys[i] = false;
			for (int i = 0; i < GLFW_KEY_LAST; i++)
				m_KeysClicked[i] = false;
			for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
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
			kb->m_Keys[key] = action;
			kb->clicked(key, action);
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Keyboard* kb = (Keyboard*)glfwGetWindowUserPointer(window);
			kb->m_MouseButtons[button] = action != GLFW_RELEASE;
		}
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			Keyboard* kb = (Keyboard*)glfwGetWindowUserPointer(window);
			kb->mx = xpos;
			kb->my = ypos;
		}

		void Keyboard::initControls()
		{
			m_Controls[Controls::up] = GLFW_KEY_UP;
			m_Controls[Controls::down] = GLFW_KEY_DOWN;
			m_Controls[Controls::left] = GLFW_KEY_LEFT;
			m_Controls[Controls::right] = GLFW_KEY_RIGHT;
			m_Controls[Controls::jump] = GLFW_KEY_SPACE;
			m_KeysControls[GLFW_KEY_UP] = Controls::up;
			m_KeysControls[GLFW_KEY_DOWN] = Controls::down;
			m_KeysControls[GLFW_KEY_LEFT] = Controls::left;
			m_KeysControls[GLFW_KEY_RIGHT] = Controls::right;
			m_KeysControls[GLFW_KEY_SPACE] = Controls::jump;
		}
	}
}
