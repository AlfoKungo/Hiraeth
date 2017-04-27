#include "camera.h"

namespace hiraeth {
	namespace view {

		Camera::Camera(graphics::Window* wnd, Timer* time)
			: m_Wnd(wnd), m_Time(time), m_PositionTimer(time->elapsed()),
			//m_Ortho(maths::mat4::LookAt(maths::vec3(0,0,-5), maths::vec3(0,0,0), maths::vec3(0,1,0)))
			m_Ortho(maths::mat4::Orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
		{
			
		}
		Camera::~Camera()
		{

		}

		void Camera::update()
		{
			if (m_Time->elapsed() - m_PositionTimer > CAMERA_TIME_BETWEEN_MOVEMENT)
			{
				maths::vec3 cpos(m_Ortho.GetPosition());
#ifdef CAMERA_RANGE
				if (m_Char->getPosition().x < cpos.x - 200)
				{
					//m_Ortho.SetPosition(m_Ortho.GetPosition() + maths::vec3(0.001f, 0, 0));
					setNewPosition(m_Char->getPosition(), maths::vec2(cpos) - maths::vec2(200,0), maths::vec2(CAMERA_X_LERP_VALUE,CAMERA_Y_LERP_VALUE));
				}
				else if (m_Char->getPosition().x > cpos.x + 200)
				{
					//m_Ortho.SetPosition(m_Ortho.GetPosition() - maths::vec3(0.001f, 0, 0));
					setNewPosition(m_Char->getPosition(), maths::vec2(cpos) + maths::vec2(200,0), maths::vec2(CAMERA_X_LERP_VALUE,CAMERA_Y_LERP_VALUE));
					//setNewPosition(maths::vec2(m_Char->getPosition().x, 0), maths::vec2(cpos.x + 200, 0), 0.025);
					//m_Ortho = m_Ortho* maths::mat4::Translate(maths::vec3(-0.01, 0, 0));
				}
				else
					setNewPosition(maths::vec2(0, m_Char->getPosition().y), maths::vec2(0, cpos.y), maths::vec2(0.0f,CAMERA_Y_LERP_VALUE));
#else
				setNewPosition(m_Char->getPosition(), cpos, maths::vec2(CAMERA_X_LERP_VALUE,CAMERA_Y_LERP_VALUE));
#endif
				m_PositionTimer = m_Time->elapsed();
			}
		}

		void Camera::setCharacter(graphics::Renderable2D* character)
		{
			m_Char = character;
				setNewPosition(m_Char->getPosition(), m_Ortho.GetPosition(), 1);
		}

		void Camera::setNewPosition(maths::vec2 a, maths::vec2 b, maths::vec2 t)
		{
			//maths::mat4 Translated = maths::mat4::Translate(-maths::vec2((a - b)*t).Divide(maths::vec2(CAMERA_VP_SIZE_X_HALF, CAMERA_VP_SIZE_Y_HALF)));
			//m_Ortho *= maths::mat4::Transpose(Translated);
		}
	}
}