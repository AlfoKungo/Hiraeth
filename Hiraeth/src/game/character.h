#pragma once

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
#include "UI/ui_manager.h"
#include "keyboard/key.h"
#include "utils/calculator.hpp"
#include "basic/char_handler.h"
#include "graphics/text_box.h"

namespace hiraeth {
	namespace game {

		class Character : public Creature, public input::KeyboardEvent, public CharHandler
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
			ui::UiEquip* m_UiEquip;
			ui::UiManager* m_UiManager;
			skills::SkillManager* m_SkillManager;
			ATimer m_SkillTimer;
			std::map<size_t, unsigned int> m_SkillKeysMap;
			CharacterStats* m_CharacterStats;
			graphics::TGroup<graphics::SpritedRenderable> m_Animations;
			std::unique_ptr<graphics::SpritedRenderable> m_Animation;
			graphics::TGroup<skills::TargetedProjectile> m_ProjectileAnimations;
			std::map<unsigned int, ATimer> m_SkillsTimeouts;
			ATimer m_SkillActivationTimer{};
			//graphics::TGroup<graphics::Sprite> m_EquipsGroup;
			//std::map<SRL::EquipItemType,  graphics::Sprite> m_Equips;
			//std::map<unsigned int, std::vector<std::pair<SRL::SkillDataType, int>>> m_AttackModifiers;
			std::map<unsigned int, std::vector<std::tuple<SRL::SkillDataType, int>>> m_ComplexEffects;

			bool m_IsStuck{ false };
		public:
			//network::ClientHandler* m_ClientHandler;

		public:
			Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer* map_layer,
				ui::UiEquip* ui_equip,ui::UiManager * ui_manager,
				item::ItemManager* item_manager, skills::SkillManager* skill_manager,
				CharacterStats* character_stats, std::map<unsigned int, Monster*>* monsters);
			~Character();

			void update() override;
			void checkMobsCollision();
			void draw(graphics::Renderer* renderer) const override;

			void ButtonClicked(input::Key control) override;
			void ButtonReleased(input::Key control) override;
			void ButtonUpdate(input::Key control, bool state);

			void registerKeys();
			void loadSkillsToKeys();
			//void setMonsters(std::vector<Monster*>* monsters_layer) { m_MonstersLayer = monsters_layer; }
			void carryOutAttack() override;
			void setStuck(bool stuck_state);
			maths::vec2 getForce() const { return m_Force; }
			network::Direction getDirection() const { return static_cast<network::Direction>(m_Direction); }
			void attackMonster(Creature* attacked, Damage d) override;
			void wearItem(SRL::EquipItemType item_type) override;
			void unWearItem(SRL::EquipItemType item_type) override;
			void writeSay(std::string say_msg) override;
			std::map<SRL::EquipItemType, graphics::Sprite>* getEquips() { return &m_Equips; }
			
			//void updateComplexSkills();
			//CharacterStats* getCharacterStats() const;
		private:
			//void cause_damage(Damage damage) override { m_Stats->causeDamage(damage); }
			void attackMonsters(std::vector<Monster*> mobs, std::vector<Damage> damages);
			void pickItemUp();
			int getValueFromString(std::string str, unsigned int val);
			int getInt(SRL::SkillPropertiesVar val, unsigned int skill_lvl);
			void activateSkill(unsigned int skill_id);
			void activateTeleport(SRL::SkillPropertiesMap* skill_properties, unsigned int skill_lvl);
			std::vector<std::pair<float, Monster*>> findMonstersInRange(maths::vec2 atk_range) const;
			std::vector<network::MonsterHit> activateAttackSkill(SRL::FullAnimationData hit_animation_data,
				int range, const std::string& skill_name);
			std::vector<network::MonsterHit> activateProjSkill(SRL::FullAnimationData hit_animation_data,
				const SRL::FullAnimationData& ball_animation_data,
				int range, const std::string& skill_name);
			//void activateAttackSkill(SRL::FullAnimationData hit_animation_data,
			//	SRL::FullAnimationData ball_animation_data, const std::string& skill_name);
		};
	}
}
