#pragma once
#include <cereal\cereal.hpp>
#include <cereal\types\vector.hpp>
#include <memory>
#include <vector>
#include "maths/maths.h"


namespace SRL {

	using namespace maths;
	typedef unsigned int NpcIndex;

	struct TileData
	{
		vec2 position;
		float scale{};
		unsigned int type{};
		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(CEREAL_NVP(position), CEREAL_NVP(scale), CEREAL_NVP(type));
		}
	};
	struct FootHoldData
	{
		vec2 p1, p2;
		vec2 direction;
		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(CEREAL_NVP(p1), CEREAL_NVP(p2), CEREAL_NVP(direction));
		}
	};
	struct PortalData
	{
		vec2 position;
		int next_map;
		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(CEREAL_NVP(position), CEREAL_NVP(next_map));
		}
	};
	struct Summon
	{
		unsigned int monster_type;
		vec2 position;
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(monster_type), CEREAL_NVP(position));
		}
	};


	struct MapData
	{
		std::vector<PortalData> Portals;
		std::vector<Summon> Summons;
		std::vector<NpcIndex> Npcs;
		std::vector<TileData> Tiles;
		std::vector<FootHoldData> FootHolds;
		int TileTexture = 1;
	private:
		MapData() = default;
		friend class cereal::access;
		template<class A> void serialize(A& ar) {
			ar( CEREAL_NVP(Portals),
				CEREAL_NVP(Summons),
				CEREAL_NVP(Npcs),
				CEREAL_NVP(Tiles),
				CEREAL_NVP(FootHolds),
				CEREAL_NVP(TileTexture));
		}
	};
}