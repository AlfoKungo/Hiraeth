#pragma once

#include <ctime>
#include "game/creature.h"
#include "keyboard/keyboard_event.h"
#include <random>
#include "monster_stats.h"
#include "graphics/sprite.h"

namespace hiraeth {
	namespace game {

			struct MonsterData
			{
				std::string monster_name;
				unsigned int stand_frames, walk_frames, hit_frames;
				MonsterStatsStruct stats;

				template<class A> void serialize(A& ar) {
					ar(monster_name, stand_frames, walk_frames, hit_frames, stats);
				}
			};
		class Monster : public Creature
		{
		private:
			std::random_device rd;
			std::mt19937 gen;
			std::uniform_int_distribution<> dis;
			float m_AiTimer;
			graphics::Sprite m_Hp;
			unsigned int m_Type;

		public:
			maths::vec2 m_StartingPosition;
			bool died = false;
			Monster(MonsterData monster_data, maths::vec2 pos, unsigned int type, map::MapLayer* mapLayer);
			virtual ~Monster() {}

			void update() override;
			void draw(graphics::Renderer* renderer) const override;
			bool checkCollision(const maths::Rectangle& rec) const;
			void attack() override {}
			
			unsigned int getType() const { return m_Type; }

		private:
			void causeDamage(Damage damage) override;
			void killMonster();
		};
	}
}
