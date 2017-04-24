#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec3 pos, Timer* time, input::Keyboard* kb,
			map::MapLayer* m_MapLayer)
			: Creature(pos, time, kb, m_MapLayer),
			m_Kb(kb)
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

				if (m_Kb->isKeyPressed(GLFW_KEY_RIGHT))
					c.right = true;
				else if (m_Kb->isKeyPressed(GLFW_KEY_LEFT))
					c.left = true;
				if (m_Kb->isKeyPressed(GLFW_KEY_UP))
					c.up = true;
				if (m_Kb->isKeyPressed(GLFW_KEY_DOWN))
					c.down = true;
				if (m_Kb->isKeyPressed(GLFW_KEY_SPACE))
					c.jump = true;
			return c;
		}

	}
}
