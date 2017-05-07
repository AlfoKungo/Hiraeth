#pragma once

#include "game\creature.h"
#include "keyboard\keyboard_event.h"

namespace hiraeth {
	namespace game {

		class Character : public Creature, public input::KeyboardEvent
		{

		private:
			input::Keyboard* m_Kb;
		public:
			Character(maths::vec2 pos, input::Keyboard* kb, 
				map::MapLayer* m_MapLayer);
			~Character();

			//void update() override;
			void ButtonClicked(input::Controls control);
			void ButtonReleased(input::Controls control);

			void registerKeys();
		private:
		};
	}
}