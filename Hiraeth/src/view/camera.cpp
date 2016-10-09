#include "camera.h"

namespace hiraeth {
	namespace view {

		Camera::Camera(graphics::Window* wnd)
			: m_Wnd(wnd), m_Ortho(maths::mat4::orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
		{
			
		}
		Camera::~Camera()
		{

		}

		void Camera::update()
		{
			if (m_Wnd->isKeyPressed(GLFW_KEY_LEFT))
				m_Ortho = m_Ortho* maths::mat4::translation(maths::vec3(0.1, 0, 0));
			if (m_Wnd->isKeyPressed(GLFW_KEY_RIGHT))
				m_Ortho = m_Ortho* maths::mat4::translation(maths::vec3(-0.1, 0, 0));
			if (m_Wnd->isKeyPressed(GLFW_KEY_UP))
				m_Ortho = m_Ortho* maths::mat4::translation(maths::vec3(0, -0.1, 0));
			if (m_Wnd->isKeyPressed(GLFW_KEY_DOWN))
				m_Ortho = m_Ortho* maths::mat4::translation(maths::vec3(0, 0.1, 0));
		}
	}
}