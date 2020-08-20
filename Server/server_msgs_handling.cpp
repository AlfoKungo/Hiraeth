#include "server.h"

namespace hiraeth {
	namespace network {
		void Server::HitMob(Address sender)
		{
			//auto[client_id, monster_damage] = dsrl_types<unsigned int, MonsterDamage>(m_Buffer + 1);
			auto [client_id, monster_hit] = dsrl_types<unsigned int, MonsterHit>(m_Buffer + 1);
			//const auto monster_damage = dsrl_types<MonsterDamage>(m_Buffer + 5);
			{ // send hit update to all players (except for the one attacked)
				//const auto client_id = dsrl_types<unsigned int>(m_Buffer + 1);
				m_Size = construct_server_packet(m_Buffer, MSG_STC_MOB_HIT, monster_hit);
				sendDataToAllClientsExcept(m_Size, client_id);
			}
			// update mob hp
			if (m_MobManager.damageMob(monster_hit))
			{
				MobDied(client_id, monster_hit.monster_id);
				//auto dead_pos = m_MobManager.monsterDied(monster_hit.monster_id);
				////std::vector<unsigned int> dropped_items {0};
				////auto[data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_damage.monster_id, {0} });
				////m_ItemsDropped[m_ItemsIdCounter] = ItemDropMsg{ m_ItemsIdCounter ,0, 0, dead_pos };
				////auto[data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_hit.monster_id, {m_ItemsDropped[m_ItemsIdCounter] } });
				////m_ItemsIdCounter++;
				//auto [data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_hit.monster_id,
				//	{addItem(0, 0, dead_pos)} });
				//m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_MOB_DIED, *data, size);
				//sendDataToAllClients(m_Size);
				//unsigned int exp = 30;
				//m_Size = construct_server_packet(m_Buffer, MSG_STC_RECEIVED_EXP, exp);
				//m_Socket.Send(sender, m_Buffer, m_Size);
			}
			//updateMonstersHp()
		}
	}
}
