#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec3 pos, Timer* time, input::Keyboard* kb,
			map::MapLayer* m_MapLayer)
			: Creature(pos, time, kb, m_MapLayer)
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

				if (m_Kb->isKeyPressed(GLFW_KEY_D))
				{
					c.right = true;
				}
				else if (m_Kb->isKeyPressed(GLFW_KEY_A))
				{
					c.left = true;
				}
				if (m_Kb->isKeyPressed(GLFW_KEY_W))
					c.up = true;
				if (m_Kb->isKeyPressed(GLFW_KEY_S))
					c.down = true;
			return c;
		}

	}
}
