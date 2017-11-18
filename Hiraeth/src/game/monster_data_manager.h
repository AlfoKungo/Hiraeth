#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <istream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include "monster_stats.h"
#include "map/map_data.h"

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

		class MonsterDataManager
		{
		private:
			static std::map<unsigned int, MonsterData> m_MonsterData;
		public:
			static const MonsterData& Get(unsigned int index);
			static void ReloadData(std::vector<map::Summon> monster_indexes);
		private:
			static MonsterData deserialize_monster_data(unsigned int monster_index);

		private:
			MonsterDataManager();
		};
	}
}