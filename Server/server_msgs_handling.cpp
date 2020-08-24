#include "server.h"

namespace hiraeth {
	namespace network {
		void Server::HitMob(Address sender)
		{
			//auto[client_id, monster_damage] = dsrl_types<unsigned int, MonsterDamage>(m_Buffer + 1);
			auto [player_id, monster_hit] = dsrl_types<unsigned int, MonsterHit>(SocketHandler::m_Buffer + 1);
			//const auto monster_damage = dsrl_types<MonsterDamage>(m_Buffer + 5);

			// send hit update to all players (except for the one attacked)
				//const auto client_id = dsrl_types<unsigned int>(m_Buffer + 1);
			SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_MOB_HIT, monster_hit);
			sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], SocketHandler::m_Size, player_id);
			// update mob hp
			//if (m_MobManager.damageMob(monster_hit))
			auto map_id = m_PlayerToMapId[player_id];
			if (m_MapHolder[map_id].mob_manager.damageMob(monster_hit))
			{
				MobDied(map_id, player_id, monster_hit.monster_id);
			}
			//updateMonstersHp()
		}
	}
}
