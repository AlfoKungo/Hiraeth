#pragma once

#include "game/creature.h"
#include "keyboard/keyboard_event.h"
#include "character_stats.h"

namespace hiraeth {
	namespace game {

		class Character : public Creature, public input::KeyboardEvent
		{

		private:
			input::Keyboard* m_Kb;
		public:
			Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer);
			~Character();

			//void update() override;
			void ButtonClicked(input::Controls control) override;
			void ButtonReleased(input::Controls control) override;

			void registerKeys();
			CharacterStats* getCharacterStats() const;
		};
	}
}
