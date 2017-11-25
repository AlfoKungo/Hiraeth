#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <istream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include "monster_stats.h"
#include "srl/map_data.h"
#include "srl/monster_data.h"
#include "srl/deserial.h"

namespace hiraeth {
	namespace game {

		class MonsterDataManager
		{
		private:
			static std::map<unsigned int, SRL::MonsterData> m_MonsterData;
		public:
			static const SRL::MonsterData& Get(unsigned int index);
			static void ReloadData(std::vector<SRL::Summon> monster_indexes);
		private:
			static SRL::MonsterData deserialize_monster_data(unsigned int monster_index);

		private:
			MonsterDataManager();
		};
	}
}