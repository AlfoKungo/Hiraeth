#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "keyboard/keyboard.h"

namespace hiraeth {
	namespace graphics {

		const int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;
		class Window
		{
		private:
			const char *m_Title;
			int m_Width, m_Height;
			GLFWwindow *m_Window;
			input::Keyboard *m_Keyboard;
			bool m_Closed;


		public:
			Window(const char *name, int width, int height, input::Keyboard *keyboard);
			~Window();
			void clear() const;
			bool closed() const;
			void update(); 
			int getWidth() const { return m_Width; }
			int getHeight() const { return m_Height; }

			input::Keyboard* getKeyboard() { return m_Keyboard; }
			void setTitle(const char* title);
		private:
			bool init();
			friend void window_resize(GLFWwindow* window, int width, int height);
		};
}


}