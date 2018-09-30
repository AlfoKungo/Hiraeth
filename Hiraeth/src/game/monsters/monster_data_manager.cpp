#include "monster_data_manager.h"

namespace hiraeth {
	namespace game {
			std::map<unsigned int, SRL::MonsterData> MonsterDataManager::m_MonsterData;

			const SRL::MonsterData& MonsterDataManager::Get(unsigned int index) 
			{
				return m_MonsterData[index]; 
			}

			void MonsterDataManager::ReloadData(std::vector<SRL::Summon> monster_indexes)
			{
				m_MonsterData.clear();
				for (auto summon : monster_indexes)
				{
					m_MonsterData[summon.monster_type] = deserialize_monster_data(summon.monster_type);
				}
			}

			SRL::MonsterData MonsterDataManager::deserialize_monster_data(unsigned int monster_index)
			{
				return SRL::deserial<SRL::MonsterData>("serialized/monster.data", monster_index - 1);
			}
	}
}