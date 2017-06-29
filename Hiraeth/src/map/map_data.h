#pragma once
#include "tile.h"
#include "physics/foothold.h"
#include <vector>
#include "portal.h"
#include "cereal/types/vector.hpp"
#include "cereal/types/queue.hpp"

namespace hiraeth {
	namespace map {
		struct Summon
		{
			unsigned int monsterType;
			maths::vec2 position;
			template<class A> void serialize(A& ar) {
				ar(monsterType, position);
			}
		};

		struct MapData
		{
			std::vector<std::unique_ptr<Tile>> m_Tiles;
			std::vector<physics::FootHold> m_FootHolds;
			std::vector<std::unique_ptr<Portal>> m_Portals;
			std::vector<Summon> m_Summons;
		private:
			MapData() = default;
			friend class cereal::access;
			template<class A> void serialize(A& ar) {
				ar(m_Tiles, m_FootHolds, m_Portals, m_Summons);
			}
		};
	}
}
