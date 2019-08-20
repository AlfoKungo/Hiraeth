#pragma once

#include "network/client_handler.h"

#include <memory>
#include "game/creature.h"
#include "keyboard/keyboard_event.h"
#include "character_stats.h"
#include "character/character_body.h"
#include "character/character_arm.h"
#include "monsters/monster.h"
#include "graphics/layers/layer.h"
#include "item/item_manager.h"
#include "skills/skill_manager.h"
#include "keyboard/key.h"
#include "utils/calculator.hpp"

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
				controls_size,
			};

		private:
			input::Keyboard* m_Kb;
			std::map<unsigned int, Monster*>* m_Monsters;
			item::ItemManager* m_ItemManager;
			skills::SkillManager* m_SkillManager;
			ATimer m_SkillTimer;
			std::map<size_t, unsigned int> m_SkillKeysMap;
			CharacterStats * m_CharacterStats;
			graphics::TGroup<graphics::SpritedRenderable> m_Animations;
			std::unique_ptr<graphics::SpritedRenderable> m_Animation;
			std::map<unsigned int, ATimer> m_SkillsTimeouts;
			ATimer m_SkillActivationTimer{};

			bool m_IsStuck{ false };
		public:
			network::ClientHandler * m_ClientHandler;

		public:
			Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer,
				item::ItemManager *item_manager, skills::SkillManager *skill_manager,
				CharacterStats * character_stats, std::map<unsigned int, Monster*>* monsters, 
				network::ClientHandler * client_handler);
			~Character();

			void update() override;
			void checkMobsCollision();
			void draw(graphics::Renderer* renderer) const override
			{
				Creature::draw(renderer);
				m_Animations.draw(renderer);
				if (m_Animation)
				{
					renderer->push(m_TransformationMatrix);
					m_Animation->draw(renderer);
					renderer->pop();
				}
			}

			void ButtonClicked(input::Key control) override;
			void ButtonReleased(input::Key control) override;
			void ButtonUpdate(input::Key control, bool state);

			void registerKeys();
			//void setMonsters(std::vector<Monster*>* monsters_layer) { m_MonstersLayer = monsters_layer; }
			void carryOutAttack() override;
			void setStuck(bool stuck_state);
			maths::vec2 getForce() const { return m_Force; }
			network::Direction getDirection() const { return static_cast<network::Direction>(m_Direction); }
			//CharacterStats* getCharacterStats() const;
		private:
			//void cause_damage(Damage damage) override { m_Stats->causeDamage(damage); }
			void pickItemUp();
			int getValueFromString(std::string str, unsigned int val);
			void activateSkill(unsigned int skill_index);
			std::vector<network::MonsterHit> activateAttackSkill(SRL::FullAnimationData hit_animation_data,
				SRL::FullAnimationData ball_animation_data, const std::string& skill_name);
			//void activateAttackSkill(SRL::FullAnimationData hit_animation_data,
			//	SRL::FullAnimationData ball_animation_data, const std::string& skill_name);
		};
	}
}
