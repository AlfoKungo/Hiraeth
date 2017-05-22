#pragma once

#include "character.h"
#include <ctime>
#include <random>
#include "monster_stats.h"

namespace hiraeth {
	namespace game {

		class Monster : public Creature
		{
		private:
			std::random_device rd;
			std::mt19937 gen;
			std::uniform_int_distribution<> dis;
			float m_AiTimer;

		public:
			Monster(maths::vec2 pos, map::MapLayer* mapLayer);
			virtual ~Monster() {}

			void update() override;
			bool checkCollision(const maths::Rectangle& rec) const;
		private:
		};
	}
}
