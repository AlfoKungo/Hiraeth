#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec3 pos, Timer* time, graphics::Window* wnd,
			map::MapLayer* m_MapLayer)
			: Creature(pos, time, wnd, m_MapLayer)
		{

		}

		Character::~Character()
			//: ~Creature()
		{

		}

		//void Character::update()
		//{
		//	Creature::update();
		//}
		CreatureControls Character::set_update_controls()
		{
			CreatureControls c;

				if (m_Wnd->isKeyPressed(GLFW_KEY_D))
				{
					c.right = true;
				}
				else if (m_Wnd->isKeyPressed(GLFW_KEY_A))
				{
					c.left = true;
				}
				if (m_Wnd->isKeyPressed(GLFW_KEY_W))
					c.up = true;
				if (m_Wnd->isKeyPressed(GLFW_KEY_S))
					c.down = true;
			return c;
		}

	}
}
