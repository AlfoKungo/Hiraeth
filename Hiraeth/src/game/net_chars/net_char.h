#pragma once
#include "game/creature.h"
#include "graphics/layers/t_group.h"
#include "character/character_body.h"
#include "character/character_arm.h"
#include "game/character_stats.h"
#include "skills/skill_manager.h"
#include "net/protocol.h"

namespace hiraeth {
	namespace game {

		class NetChar : public Creature
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
			//input::Keyboard* m_Kb;
			//std::vector<Monster*>* m_MonstersLayer;
			//item::ItemManager* m_ItemManager;
			skills::SkillManager* m_SkillManager;
			ATimer m_SkillTimer;
			std::map<size_t, unsigned int> m_SkillKeysMap;
			//CharacterStats * m_CharacterStats;
			graphics::TGroup<graphics::SpritedRenderable> m_Animations;
			std::unique_ptr<graphics::SpritedRenderable> m_Animation;
			std::map<unsigned int, ATimer> m_SkillsTimeouts;
			ATimer m_SkillActivationTimer{};

			bool m_IsStuck{false};

		public:
			NetChar(maths::vec2 pos, map::MapLayer *map_layer, skills::SkillManager *skill_manager);
			~NetChar();

			void update() override;
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

			//void setMonsters(std::vector<Monster*>* monsters_layer) { m_MonstersLayer = monsters_layer; }
			inline void setState(const network::PlayerStateUpdate& state)
			{
				setPosition(state.pos);
				m_Force = state.force;
				m_Direction = static_cast<Direction>(state.direction);
			}
			inline void setPosition(const maths::vec2& position) override
			{
				const maths::vec2 old_pos = m_Bounds.position;
				m_Bounds.position = position;
				m_TransformationMatrix *= maths::mat4::Translate(position - old_pos);
			}
			virtual float calculate_air_modifier() const override 
			{
				return 1.0f;
			}
			void ButtonUpdate(input::Key key, bool state);
			void carryOutAttack() override;
			void setStuck(bool stuck_state);
			//CharacterStats* getCharacterStats() const;
		private:
			//void cause_damage(Damage damage) override { m_Stats->causeDamage(damage); }
			void pickItemUp();
			void activateSkill(unsigned int skill_index);
			void activateAttackSkill( SRL::FullAnimationData hit_animation_data, 
				SRL::FullAnimationData ball_animation_data, const std::string& skill_name);
			
		};
	}
}
