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

		public:
			SRL::Summon m_Summon;
			bool died = false;
			Monster(const SRL::MonsterData& monster_data, SRL::Summon summon, map::MapLayer* mapLayer);

			Monster(SRL::Summon summon, map::MapLayer* map_layer) :
				Monster(MonsterDataManager::Get(summon.monster_type), summon, map_layer)
			{
			}

			virtual ~Monster() { delete m_Stats; }

			void update() override;
			void draw(graphics::Renderer* renderer) const override;
			bool check_collision(const maths::Rectangle& rec) ;

			void attack() override
			{
			}

			void getHit(Direction dir, Damage damage) override;
			SRL::Summon get_summon() const { return m_Summon; }
			//maths::Rectangle getHitBox() { return }

		private:
			void cause_damage(Damage damage) override;
			void kill_monster();
		};
	}
}
