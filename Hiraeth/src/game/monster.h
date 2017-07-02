#pragma once

#include <ctime>
#include <fstream>
#include <random>
#include "game/creature.h"
#include "keyboard/keyboard_event.h"
#include "monster_stats.h"
#include "graphics/sprite.h"

namespace hiraeth {
	namespace game {

		struct MonsterFramesAmount
		{
			unsigned int stand_frames, walk_frames, hit_frames;
			template<class A> void serialize(A& ar) {
				ar(stand_frames, walk_frames, hit_frames);
			}
		};
		struct MonsterData
		{
			std::string monster_name;
			MonsterFramesAmount monster_frames_amount;
			MonsterStatsStruct stats;

			template<class A> void serialize(A& ar) {
				ar(monster_name, monster_frames_amount, stats);
			}
		};
		class Monster : public Creature
		{
		private:
			std::random_device rd;
			std::mt19937 gen;
			std::uniform_int_distribution<> dis;
			ATimer m_AiTimer;
			graphics::Sprite m_Hp;

		public:
			map::Summon m_Summon;
			bool died = false;
			Monster(MonsterData monster_data, map::Summon summon, map::MapLayer* mapLayer);
			Monster(map::Summon summon, map::MapLayer* mapLayer) :
				Monster(deserialize_monster_data(summon.monsterType), summon, mapLayer) {}
			virtual ~Monster() {}

			void update() override;
			void draw(graphics::Renderer* renderer) const override;
			bool checkCollision(const maths::Rectangle& rec) const;
			void attack() override {}

			void getHit(Direction dir, Damage damage) override;
			map::Summon getSummon() const { return m_Summon; }

		private:
			MonsterData deserialize_monster_data(unsigned int monster_index);
			void causeDamage(Damage damage) override;
			void killMonster();
		};
	}
}
