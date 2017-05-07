#pragma once

#include "character.h"

namespace hiraeth {
	namespace game {

		class Monster : public Creature
		{
		private:
			unsigned int level, maxHP, maxMP;

		public:
			Monster(maths::vec2 pos, map::MapLayer* mapLayer);
			virtual ~Monster() {}

			//void update() override;
			void update() override;
		private:
		};
	}
}
