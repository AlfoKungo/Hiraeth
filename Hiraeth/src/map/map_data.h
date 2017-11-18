#pragma once
#include <vector>
#include "cereal/types/vector.hpp"
#include "cereal/types/queue.hpp"
#include "maths/maths.h"

namespace hiraeth {
	namespace map {
		struct TileData
		{
			maths::vec2 position;
			float scale;
			//maths::vec2 uv_pos;
			//maths::vec2 uv_size;
			unsigned int type;
			template<class Archive>
			void serialize(Archive & ar)
			{
				ar(position, scale, type);
			}
		};
		struct FootHoldData
		{
			maths::vec2 p1, p2;
			maths::vec2 m_Direction;
			template<class Archive>
			void serialize(Archive & ar)
			{
				ar(p1, p2, m_Direction);
			}
		};
		struct PortalData
		{
			maths::vec2 position;
			int next_map;
			template<class Archive>
			void serialize(Archive & ar)
			{
				ar(position, next_map);
			}
		};
		struct Summon
		{
			unsigned int monsterType;
			maths::vec2 position;
			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(monsterType, position);
			}
		};


		struct MapData
		{
			std::vector<TileData> m_Tiles;
			std::vector<FootHoldData> m_FootHolds;
			std::vector<PortalData> m_Portals;
			std::vector<Summon> m_Summons;
			int m_TileTexture;
		private:
			MapData() = default;
			friend class cereal::access;
			template<class A> void serialize(A& ar) {
				ar(m_Tiles, m_FootHolds, m_Portals, m_Summons, m_TileTexture);
			}
		};
	}
}
