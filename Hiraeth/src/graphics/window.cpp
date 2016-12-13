#include "window.h"

namespace hiraeth {
	namespace graphics {

		Window::Window(const char *title, int width, int height)
		{
			m_Title = title;
			m_Width = width;
			m_Height = height;

			if (!init())
				glfwTerminate();
			glfwSetWindowPos(m_Window, (1920 - width) / 2, (1080 - height) / 2);

			for (int i = 0; i < GLFW_KEY_LAST; i++)
			{
				m_Keys[i] = false;
				m_LastKeys[i] = false;
				m_KeyTyped[i] = false;
			}
			for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
			{
				m_MouseButtons[i] = false;
				m_MouseState[i] = false;
				m_MouseClicked[i] = false;
			}
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
			glfwSetWindowUserPointer(m_Window, this);
			glfwSetFramebufferSizeCallback(m_Window, window_resize);
			glfwSetKeyCallback(m_Window, key_callback);
			glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
			glfwSetCursorPosCallback(m_Window, cursor_position_callback);
			// Disable VSYNC
			glfwSwapInterval(0.0);

			glEnable(GL_BLEND); 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			glClearColor(0.0, 0.0, 0.0, 0.0);

			if (glewInit() != GLEW_OK)
			{
				std::cout << "COULD NOT INIT GLEw!" ;
				return false;
			}

			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

			return true;
		}

		bool Window::closed() const
		{
			return glfwWindowShouldClose(m_Window) == 1;
		}
		
		bool Window::isKeyPressed(unsigned int keycode) const
		{
			if (keycode >= GLFW_KEY_LAST)
				return false;

			return m_Keys[keycode];
		}
		
		bool Window::isKeyTyped(unsigned int keycode) const
		{
			if (keycode >= GLFW_KEY_LAST)
				return false;

			return m_KeyTyped[keycode];
		}
		
		bool Window::isMouseButtonPressed(unsigned int button) const
		{
			if (button >= GLFW_MOUSE_BUTTON_LAST)
				return false;

			return m_MouseButtons[button];
		}

		bool Window::isMouseButtonClicked(unsigned int button) const
		{
			if (button >= GLFW_MOUSE_BUTTON_LAST)
				return false;

			return m_MouseClicked[button];
		}

		void Window::getMousePosition(double& x, double& y) const
		{
			x = mx;
			y = my;
		}

		 void Window::clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::update() 
		{
			for (int i = 0; i < GLFW_KEY_LAST; i++)
				m_KeyTyped[i] = m_Keys[i] && !m_LastKeys[i];

			for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
				m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

			memcpy(m_LastKeys, m_Keys, GLFW_KEY_LAST);
			memcpy(m_MouseState, m_MouseButtons, GLFW_MOUSE_BUTTON_LAST);

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

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->m_Keys[key] = action;
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->m_MouseButtons[button] = action != GLFW_RELEASE;
		}
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->mx = xpos;
			win->my = ypos;
		}
	}


}