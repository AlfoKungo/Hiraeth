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

namespace hiraeth {
	namespace game {

		class Monster : public Creature
		{
		private:
			std::random_device rd;
			std::mt19937 gen;
			std::uniform_int_distribution<> dis;
			ATimer m_AiTimer;
			graphics::Sprite m_Hp;
			float m_XStart, m_XEnd;

		public:
			map::Summon m_Summon;
			bool died = false;
			Monster(const MonsterData& monster_data, map::Summon summon, map::MapLayer* mapLayer);
			Monster(map::Summon summon, map::MapLayer* mapLayer) :
				Monster(MonsterDataManager::Get(summon.monsterType), summon, mapLayer) {}
			virtual ~Monster() { delete m_Stats; }

			void update() override;
			void draw(graphics::Renderer* renderer) const override;
			bool checkCollision(const maths::Rectangle& rec) const;
			void attack() override {}

			void getHit(Direction dir, Damage damage) override;
			map::Summon getSummon() const { return m_Summon; }

		private:
			void causeDamage(Damage damage) override;
			void killMonster();
		};
	}
}
