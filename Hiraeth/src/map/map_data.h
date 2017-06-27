#pragma once
#include "tile.h"
#include "physics/foothold.h"
#include <vector>
#include "portal.h"
#include "cereal/types/vector.hpp"

namespace hiraeth {
	namespace map {
		struct MapData
		{
			std::vector<std::unique_ptr<Tile>> m_Tiles;
			std::vector<physics::FootHold> m_FootHolds;
			std::vector<std::unique_ptr<Portal>> m_Portals;
		private:
			MapData() = default;
			friend class cereal::access;
			template<class A> void serialize(A& ar) {
				ar(m_Tiles, m_FootHolds, m_Portals);
			}
		};
	}
}
