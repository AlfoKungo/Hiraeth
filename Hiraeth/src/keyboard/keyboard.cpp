#include "keyboard.h"

namespace hiraeth {
	namespace input {

		Keyboard::Keyboard()
		{

			for (int i = 0; i < GLFW_KEY_LAST; i++)
				keys_map.insert(std::pair<unsigned int, KeyboardEvent*>(i, nullptr));
			for (int i = 0; i < GLFW_KEY_LAST; i++)
				m_Keys[i] = false;
			for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
				m_MouseButtons[i] = false;
		}
		bool Keyboard::isKeyPressed(unsigned int keycode) const
		{
			if (keycode >= GLFW_KEY_LAST)
				return false;

			return m_Keys[keycode];
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
			Keyboard* kb = (Keyboard*) glfwGetWindowUserPointer(window);
			kb->m_Keys[key] = action;
			kb->clicked(key, action);
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Keyboard* kb = (Keyboard*) glfwGetWindowUserPointer(window);
			kb->m_MouseButtons[button] = action != GLFW_RELEASE;
		}
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			Keyboard* kb = (Keyboard*) glfwGetWindowUserPointer(window);
			kb->mx = xpos;
			kb->my = ypos;
		}
	}
}
