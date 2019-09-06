#pragma once

#include <ctime>
#include <fstream>
#include <random>
#include "game/creature.h"
#include "keyboard/keyboard_event.h"
#include "monster_stats.h"
#include "graphics/sprite.h"
#include "monster_data_manager.h"
#include "monster_data_manager.h"
#include "srl/deserial.h"
#include "graphics/layers/t_group.h"
#include "game/targeted_projectile.h"

namespace hiraeth
{
	namespace game
	{
		class Monster : public Creature
		{
		private:
			graphics::Sprite m_Hp;
			float m_XStart, m_XEnd;
			maths::vec2 m_HitBox;
			MonsterStats m_MonsterStats;
			SRL::MonsterStatsStruct* m_StatsStruct;
			graphics::TGroup<graphics::SpritedRenderable> m_Animations;
			graphics::TGroup<skills::TargetedProjectile> m_ProjectileAnimations;
			bool m_HasFinished{ false };
			unsigned int m_Id{};

		public:
			//SRL::Summon m_Summon;
			Monster(const SRL::MonsterData& monster_data, maths::vec2 position, map::MapLayer* mapLayer, unsigned int mob_id,
				float x_start, float x_end);

			Monster(unsigned int monster_type, maths::vec2 position, map::MapLayer* map_layer, unsigned int mob_id,
				float x_start, float x_end) :
				Monster(MonsterDataManager::Get(monster_type), position, map_layer, mob_id,
				 x_start,x_end)
			{
			}

			//Monster(const SRL::MonsterData& monster_data, SRL::Summon summon, map::MapLayer* mapLayer);
			//Monster(SRL::Summon summon, map::MapLayer* map_layer) :
			//	Monster(MonsterDataManager::Get(summon.monster_type), summon, map_layer)
			//{
			//}

			virtual ~Monster() { }

			void update() override;
			void draw(graphics::Renderer* renderer) const override;
			bool check_collision(const maths::Rectangle& rec) ;

			void carryOutAttack() override
			{
			}
			bool hasSpriteFinished() const override
			{
				return m_HasFinished;
			}

			void setControls(unsigned int control_id, bool control_state)
			{
				if (control_id == 0)
					m_Controls.left = control_state;
				else
				{
					m_Controls.right = control_state;
					printf("the location is %f : %f\n", m_Bounds.x, m_Bounds.y);
				}
			}
			void getHit(Direction dir, Damage damage) override;
			void setHitAnimation(std::unique_ptr<graphics::SpritedRenderable> hit_animation);
			void setProjectileAnimation(std::unique_ptr<skills::TargetedProjectile> projectile_animation);
			void setForceByMove(float x_force) override { m_Force.x = x_force; } 
			unsigned int getId() const { return m_Id; }
			//void change_stance(StanceState new_state) override;
			//maths::Rectangle getHitBox() { return }

			//SRL::Summon getSummon() const { return m_Summon; }
		protected:
			void multiply_by_friction(maths::vec2& force) const override
			{
				//if (m_Foothold != NO_FOOTHOLD)
				//	force *= FRICTION;
				//else
				//	force *= FRICTION_AIR;
				force *= 1;
			}
		private:
			void cause_damage(Damage damage) override;
			void triggerDeathAnimation();
		};
	}
}
