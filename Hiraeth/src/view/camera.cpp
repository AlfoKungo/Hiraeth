#include "camera.h"

namespace hiraeth {
	namespace view {

		maths::mat4 Camera::m_Ortho;
		const graphics::Renderable* Camera::m_Char;
		float Camera::m_PositionTimer;

		void Camera::init(graphics::Renderable* character)
		{
			m_Ortho = maths::mat4(maths::mat4::Orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f));
			m_PositionTimer = StaticTimer::timer.elapsed();
			setCharacter(character);
		}

		void Camera::update()
		{
			if (StaticTimer::timer.elapsed() - m_PositionTimer > CAMERA_TIME_BETWEEN_MOVEMENT)
			{
				maths::vec3 cpos(m_Ortho.GetPosition());
#ifdef CAMERA_RANGE
				if (m_Char->getPosition().x < cpos.x - 200)
				{
					//m_Ortho.SetPosition(m_Ortho.GetPosition() + maths::vec3(0.001f, 0, 0));
					setNewPosition(m_Char->getPosition(), maths::vec2(cpos) - maths::vec2(200, 0), maths::vec2(CAMERA_X_LERP_VALUE, CAMERA_Y_LERP_VALUE));
				}
				else if (m_Char->getPosition().x > cpos.x + 200)
				{
					//m_Ortho.SetPosition(m_Ortho.GetPosition() - maths::vec3(0.001f, 0, 0));
					setNewPosition(m_Char->getPosition(), maths::vec2(cpos) + maths::vec2(200, 0), maths::vec2(CAMERA_X_LERP_VALUE, CAMERA_Y_LERP_VALUE));
					//setNewPosition(maths::vec2(m_Char->getPosition().x, 0), maths::vec2(cpos.x + 200, 0), 0.025);
					//m_Ortho = m_Ortho* maths::mat4::Translate(maths::vec3(-0.01, 0, 0));
				}
				else
					setNewPosition(maths::vec2(0, m_Char->getPosition().y), maths::vec2(0, cpos.y), maths::vec2(0.0f, CAMERA_Y_LERP_VALUE));
#else
				setNewPosition(m_Char->getPosition(), cpos, maths::vec2(CAMERA_X_LERP_VALUE, CAMERA_Y_LERP_VALUE));
#endif
				m_PositionTimer = StaticTimer::timer.elapsed();
			}
		}

		void Camera::setCharacter(graphics::Renderable* character)
		{
			m_Char = character;
			setNewPosition(m_Char->getPosition(), m_Ortho.GetPosition(), 1);
		}

		maths::vec2 Camera::getCharacterPos()
		{
			return m_Char->getPosition();
		}

		maths::vec2 Camera::mouse_to_map_position(maths::vec2 mouse_pos)
		{
			maths::vec2 cam_position = m_Ortho.GetPosition();
			//cam_position *= maths::vec2{ 1,1 };
			mouse_pos = { mouse_pos.x - 800, -mouse_pos.y + 450 };
			return  { mouse_pos.x + cam_position.x, (mouse_pos.y + cam_position.y) };
		}

		maths::vec2 Camera::mouse_to_screen_position(maths::vec2 mouse_pos)
		{
			mouse_pos = { mouse_pos.x - 800, -mouse_pos.y + 450 };
			return  mouse_pos;
		}

		void Camera::setNewPosition(maths::vec2 char_pos, maths::vec2 cam_pos, maths::vec2 lerp)
		{
			auto pos = -maths::vec2((char_pos - cam_pos)*lerp).Divide(maths::vec2(CAMERA_VP_SIZE_X_HALF, CAMERA_VP_SIZE_Y_HALF));
			maths::mat4 Translated = maths::mat4::Translate(pos);
			m_Ortho *= maths::mat4::Transpose(Translated);
			maths::vec3 new_pos = m_Ortho.GetPosition();
			m_Ortho.SetPosition(maths::vec3{ floor(new_pos.x), floor(new_pos.y), new_pos.z });
			;
		}
		//void Camera::setNewPosition(maths::vec2 char_pos, maths::vec2 cam_pos, maths::vec2 lerp)
		//{
		//	auto pos = -maths::vec2((char_pos - cam_pos)*lerp).Divide(maths::vec2(CAMERA_VP_SIZE_X_HALF, CAMERA_VP_SIZE_Y_HALF));
		//	//pos = maths::vec2{ round(pos.x), round(pos.y) };
		//	maths::mat4 Translated = maths::mat4::Translate(pos);
		//	m_Ortho *= maths::mat4::Transpose(Translated);
		//}
	}
}