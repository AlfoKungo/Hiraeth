#pragma once

#include "game/creature.h"
#include "keyboard/keyboard_event.h"
#include "character_stats.h"
#include "character/character_body.h"
#include "character/character_arm.h"
#include "monster.h"
#include "graphics/layers/layer.h"
#include "item/item_manager.h"

namespace hiraeth {
	namespace game {

		class Character : public Creature, public input::KeyboardEvent
		{

		private:
			input::Keyboard* m_Kb;
			std::vector<Monster*>* m_MonstersLayer;
			item::ItemManager* m_ItemManager;
		public:
			Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer, item::ItemManager *item_manager);
			~Character();

			void update() override;

			void ButtonClicked(input::Controls control) override;
			void ButtonReleased(input::Controls control) override;

			void registerKeys();
			void setMonsters(std::vector<Monster*>* monsters_layer) { m_MonstersLayer = monsters_layer; }
			void attack() override;
			CharacterStats* getCharacterStats() const;
		private:
			void causeDamage(Damage damage) override { m_Stats->causeDamage(damage); }
			void pickItemUp();
		};
	}
}
