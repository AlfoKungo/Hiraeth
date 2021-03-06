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
#include "game/projectile.h"

namespace hiraeth
{
	namespace game
	{
		class Monster : public Creature
		{
		private:
			std::random_device rd;
			std::mt19937 gen;
			std::uniform_int_distribution<> dis;
			ATimer m_AiTimer;
			graphics::Sprite m_Hp;
			float m_XStart, m_XEnd;
			maths::vec2 m_HitBox;
			MonsterStats m_MonsterStats;
			SRL::MonsterStatsStruct* m_StatsStruct;
			graphics::TGroup<graphics::SpritedRenderable> m_Animations;
			graphics::TGroup<skills::Projectile> m_ProjectileAnimations;
			bool m_HasFinished{ false };

		public:
			SRL::Summon m_Summon;
			Monster(const SRL::MonsterData& monster_data, SRL::Summon summon, map::MapLayer* mapLayer);

			Monster(SRL::Summon summon, map::MapLayer* map_layer) :
				Monster(MonsterDataManager::Get(summon.monster_type), summon, map_layer)
			{
			}

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

			void getHit(Direction dir, Damage damage) override;
			void getHit(std::unique_ptr<skills::Projectile> projectile_animation);
			SRL::Summon getSummon() const { return m_Summon; }
			//void change_stance(StanceState new_state) override;
			//maths::Rectangle getHitBox() { return }

		private:
			void cause_damage(Damage damage) override;
			void triggerDeathAnimation();
		};
	}
}
