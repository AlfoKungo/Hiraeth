#include "monster_data_manager.h"

namespace hiraeth {
	namespace game {
			std::map<unsigned int, MonsterData> MonsterDataManager::m_MonsterData;

			const MonsterData& MonsterDataManager::Get(unsigned int index) 
			{
				return m_MonsterData[index]; 
			}

			void MonsterDataManager::ReloadData(std::vector<map::Summon> monster_indexes)
			{
				m_MonsterData.clear();
				for (auto summon : monster_indexes)
				{
					m_MonsterData[summon.monsterType] = deserialize_monster_data(summon.monsterType);
				}
			}

			MonsterData MonsterDataManager::deserialize_monster_data(unsigned int monster_index)
			{
				std::ifstream file("monster.data");
				cereal::BinaryInputArchive iarchive(file);
				int start_of_data;
				file.seekg(sizeof(int) * (monster_index - 1));
				iarchive(start_of_data);
				file.seekg(start_of_data);
				MonsterData monster_data;
				iarchive(monster_data);
				return monster_data;
			}
	}
}