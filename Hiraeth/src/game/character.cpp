#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec3 pos, graphics::Texture* ptex , Timer* time, graphics::Window* wnd)
			: graphics::SpritedRenderable(pos, 3 ,0.4f, false, ptex,time), m_Wnd(wnd), m_MovementTimer(time->elapsed())
		{
			
		}

		void Character::update()
		{
			SpritedRenderable::update();
			if (m_Time->elapsed() - m_MovementTimer > TIME_BETWEEN_ADDS)
			{
				if (m_Wnd->isKeyPressed(GLFW_KEY_D))
					m_Position += maths::vec3(CHARACTER_SPEED, 0, 0);
				if (m_Wnd->isKeyPressed(GLFW_KEY_A))
					m_Position += maths::vec3(-CHARACTER_SPEED, 0, 0);
				if (m_Wnd->isKeyPressed(GLFW_KEY_W))
					m_Position += maths::vec3(0, CHARACTER_SPEED, 0);
				if (m_Wnd->isKeyPressed(GLFW_KEY_S))
					m_Position += maths::vec3(0, -CHARACTER_SPEED, 0);
				m_MovementTimer = m_Time->elapsed();
			}
		}
	}
}
