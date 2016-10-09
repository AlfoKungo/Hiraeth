#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace hiraeth {
	namespace graphics {

		class Window
		{
		private:
			const char *m_Title;
			int m_Width, m_Height;
			GLFWwindow *m_Window;
			bool m_Closed;

			bool m_Keys[GLFW_KEY_LAST];
			bool m_MouseButtons[GLFW_MOUSE_BUTTON_LAST];
			double mx, my;
		public:
			Window(const char *name, int widt, int height);
			~Window();
			void clear() const;
			bool closed() const;
			void update(); 
			int getWidth() const { return m_Width; }
			int getHeight() const { return m_Height; }

			bool isKeyPressed(unsigned int keycode) const;
			bool isMouseButtonPressed(unsigned int button) const;
			void getMousePosition(double& x, double& y) const;
		private:
			bool init();
			friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		};
}


}