#pragma once
#include "mob_animation.h"
#include "keyboard/keyboard.h"

namespace hiraeth {
	namespace editor {
		
		class MobController : public input::KeyboardEvent
		{
			MobAnimation* m_Mob;
		public:
			MobController(MobAnimation* mob, input::Keyboard* kb)
				: m_Mob{mob}
			{
				kb->registerToKey(GLFW_KEY_1, 1, this);
				kb->registerToKey(GLFW_KEY_2, 2, this);
				kb->registerToKey(GLFW_KEY_3, 3, this);
				kb->registerToKey(GLFW_KEY_4, 4, this);
			}
			virtual void ButtonClicked(input::Key control)
			{
				switch (control)
				{
				case 1:
					m_Mob->changeStanceState(game::StcStand);
					break;
				case 2:
					m_Mob->changeStanceState(game::StcWalk);
					break;
				case 3:
					m_Mob->changeStanceState(game::StcJump);
					break;
				case 4:
					m_Mob->changeStanceState(game::StcDie);
					break;
				default:
					break;
				}
			}
			virtual void ButtonReleased(input::Key control)
			{
				
			}
		};
	}
}
