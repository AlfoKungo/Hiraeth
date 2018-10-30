#pragma once
#include <map>
#include "net/protocol.h"
#include "srl/monster_data.h"

namespace hiraeth {
	namespace network {

		class MobManager
		{
			using MonsterType = unsigned int;
		private:
			std::map<unsigned int, MonsterStateUpdate> m_Monsters;
			std::map<unsigned int, MonsterType> m_MonsterIdToType;
			std::map<MonsterType, SRL::MonsterData> m_MonsterTypeToData;
		public:
			void update()
			{
				for (auto& [id, monster_struct] : m_Monsters)
				{
					auto monster_data = m_MonsterTypeToData[m_MonsterIdToType[id]];
					monster_struct.pos.x += monster_data.StatsStruct.Speed;
				}
			}
		};
	}
}
