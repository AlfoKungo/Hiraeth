#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "keyboard/keyboard.h"
#include <thread>
#include "utils/static_timer.h"

namespace hiraeth {
	namespace graphics {
		const int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;
		class Window
		{
		private:
			const char* m_Title;
			int m_Width, m_Height;
			GLFWwindow* m_Window;
			input::Keyboard* m_Keyboard;
			bool m_Closed;
	//		std::thread m_PollEvents{ [&]() {
	//std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	//			unsigned int frames = 0;
	//			float timer = 0;
	//			while (true) {
	//			glfwPollEvents();
	//			//if (m_Window != nullptr)
	//				//glfwSwapBuffers(m_Window);
	//			//std::cout << "im polling\n";
	//			//std::this_thread::sleep_for(std::chrono::milliseconds(66));
	//	//++frames;
	//	//if (StaticTimer::timer.elapsed() - timer > 1.0f)
	//	//{
	//	//	timer += 1.0f;
	//	//	printf("%d fps, %f frame time\n", frames, 1.0 / frames);
	//	//	frames = 0;
	//	//}
	//			}
	//		} };

		public:
			Window(const char* name, int width, int height, input::Keyboard* keyboard);
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