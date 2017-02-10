#include "window.h"

namespace hiraeth {
	namespace graphics {

		Window::Window(const char *title, int width, int height, input::Keyboard* keyboard)
			: m_Keyboard(keyboard)
		{
			m_Title = title;
			m_Width = width;
			m_Height = height;

			if (!init())
				glfwTerminate();
			glfwSetWindowPos(m_Window, (1920 - width) / 2, (1080 - height) / 2);

			//for (int i = 0; i < GLFW_KEY_LAST; i++)
			//{
			//	m_Keys[i] = false;
			//	m_LastKeys[i] = false;
			//	m_KeyTyped[i] = false;
			//}
			//for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
			//{
			//	m_MouseButtons[i] = false;
			//	m_MouseState[i] = false;
			//	m_MouseClicked[i] = false;
			//}
		}

		Window::~Window()
		{
			glfwTerminate();
		}

		bool Window::init() {
			if (!glfwInit())
			{
				std::cout << "Error" << std::endl;
				return false;
			}
			m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
			if (!m_Window) {
				glfwTerminate();
				std::cout << "FAILED TO CREATE glfw WINDOW" << std::endl;
				return false;
			}
			glfwMakeContextCurrent(m_Window);
			glfwSetWindowUserPointer(m_Window, m_Keyboard);
			glfwSetFramebufferSizeCallback(m_Window, window_resize);
			glfwSetKeyCallback(m_Window, input::key_callback);
			glfwSetMouseButtonCallback(m_Window, input::mouse_button_callback);
			glfwSetCursorPosCallback(m_Window, input::cursor_position_callback);

			// Disable VSYNC
			glfwSwapInterval(0.0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glClearColor(0.0, 0.0, 0.0, 0.0);

			if (glewInit() != GLEW_OK)
			{
				std::cout << "COULD NOT INIT GLEw!";
				return false;
			}

			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

			return true;
		}

		bool Window::closed() const
		{
			return glfwWindowShouldClose(m_Window) == 1;
		}


		void Window::setTitle(const char* title)
		{
			glfwSetWindowTitle(m_Window, title);
		}

		 void Window::clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::update() 
		{
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				std::cout << "OpenGL Error: " << error << std::endl;

			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}

		void window_resize(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_Width = width;
			win->m_Height = height;
		}
	}


}