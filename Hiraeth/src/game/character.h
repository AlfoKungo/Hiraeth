#pragma once

#include <memory>
#include "game/creature.h"
#include "keyboard/keyboard_event.h"
#include "character_stats.h"
#include "character/character_body.h"
#include "character/character_arm.h"
#include "monster.h"
#include "graphics/layers/layer.h"
#include "item/item_manager.h"
#include "skills/skill_manager.h"
#include "keyboard/key.h"

namespace hiraeth {
	namespace game {

		class Character : public Creature, public input::KeyboardEvent
		{
			enum Controls
			{
				up,
				down,
				left,
				right,
				jump,
				attack,
				pick_up,
			};

		private:
			input::Keyboard* m_Kb;
			std::vector<Monster*>* m_MonstersLayer;
			item::ItemManager* m_ItemManager;
			skills::SkillManager* m_SkillManager;
			ATimer m_SkillTimer;
			std::map<size_t, unsigned int> m_SkillKeysMap;
			CharacterStats * m_CharacterStats;

		public:
			Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer, item::ItemManager *item_manager,
				skills::SkillManager *skill_manager, CharacterStats * character_stats);
			~Character();

			void update() override;

			void ButtonClicked(input::Key control) override;
			void ButtonReleased(input::Key control) override;
			void ButtonUpdate(input::Key control, bool state);

			void registerKeys();
			void setMonsters(std::vector<Monster*>* monsters_layer) { m_MonstersLayer = monsters_layer; }
			void carryOutAttack() override;
			//CharacterStats* getCharacterStats() const;
		private:
			//void cause_damage(Damage damage) override { m_Stats->causeDamage(damage); }
			void pickItemUp();
			void use_skill(unsigned int skill_index);
			void activate_skill(unsigned int skill_index);
		};
	}
}
