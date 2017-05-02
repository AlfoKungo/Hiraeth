#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec3 pos, Timer* time, input::Keyboard* kb,
			map::MapLayer* m_MapLayer)
			: Creature(pos, time, kb, m_MapLayer),
			m_Kb(kb)
		{
			m_Kb->registerToKey(input::Controls::up, this);
			m_Kb->registerToKey(input::Controls::down, this);
			m_Kb->registerToKey(input::Controls::right, this);
			m_Kb->registerToKey(input::Controls::left, this);
			m_Kb->registerToKey(input::Controls::jump, this);
		}

		Character::~Character()
			//: ~Creature()
		{

		}

		//void Character::update()
		//{
		//	Creature::update();
		//}

		void Character::ButtonReleased(input::Controls control)
		{
			if (control == input::Controls::right)
				m_Controls.right = false;
			else if (control == input::Controls::left)
				m_Controls.left = false;
			if (control == input::Controls::down)
				m_Controls.down = false;
			if (control == input::Controls::jump)
				m_Controls.jump = false;
		}

		void Character::ButtonClicked(input::Controls control)
		{
			if (control == input::Controls::right)
				m_Controls.right = true;
			else if (control == input::Controls::left)
				m_Controls.left = true;
			if (control == input::Controls::down)
				m_Controls.down = true;
			if (control == input::Controls::jump)
				m_Controls.jump = true;
		}

		CreatureControls Character::set_update_controls()
		{
			CreatureControls c;

				if (m_Kb->isControlPressed(input::Controls::right))
					c.right = true;
				else if (m_Kb->isControlPressed(input::Controls::left))
					c.left = true;
				if (m_Kb->isControlPressed(input::Controls::down))
					c.down = true;
				if (m_Kb->isControlPressed(input::Controls::jump))
					c.jump = true;
			return c;
		}

	}
}
