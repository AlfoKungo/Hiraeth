#pragma once

#include "character.h"
#include <ctime>
#include <random>

namespace hiraeth {
	namespace game {

		class Monster : public Creature
		{
		private:
			unsigned int level, maxHP, maxMP;
			float timer;

		public:
			Monster(maths::vec2 pos, map::MapLayer* mapLayer);
			virtual ~Monster() {}

			void update() override;
		private:
		};
	}
}
