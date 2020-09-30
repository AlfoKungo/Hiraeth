#include "map_holder.h"

namespace hiraeth {
	namespace network {
		ItemDropData MapHolder::addDrop(unsigned int item_type_id, unsigned int item_kind,
			maths::vec2 pos)
		{
			if (item_kind == network::EQUIP_ITEM)
				return addDrop({ item_type_id, RandomizeEquipStats(item_type_id) }, pos);

			auto new_item = ItemDropData{ m_ItemsIdCounter ,item_type_id, item_kind, pos };
			items_dropped[m_ItemsIdCounter] = new_item;
			m_ExpiringQueue.push(ItemExpirer{ m_ItemsIdCounter, ATimer{ EXPIRE_TIME } });
			m_ItemsIdCounter++;
			return new_item;
		}
		ItemDropData MapHolder::addDrop(SRL::EquipDbStruct equip_info,
			maths::vec2 pos)
		{
			
			auto new_item = ItemDropData{ m_ItemsIdCounter, equip_info.equip_id, 
				network::EQUIP_ITEM, pos };
			items_dropped[m_ItemsIdCounter] = new_item;
			m_ExpiringQueue.push(ItemExpirer{ m_ItemsIdCounter, ATimer{ EXPIRE_TIME } });
			m_EquipsStats[m_ItemsIdCounter] = equip_info.equip_item_properties;
			m_ItemsIdCounter++;
			return new_item;
		}
		void MapHolder::findExpiredItems()
		{
			while (!m_ExpiringQueue.empty())
			{
				if (m_ExpiringQueue.front().expire_timer.hasExpired())
				{
					if (items_dropped.find(m_ExpiringQueue.front().item_id) != items_dropped.end())
					{
						sendItemExpired(m_ExpiringQueue.front().item_id);
						if (m_EquipsStats.find(m_ExpiringQueue.front().item_id) != m_EquipsStats.end())
							m_EquipsStats.erase(m_ExpiringQueue.front().item_id);
					}
					m_ExpiringQueue.pop();
				}
				else
					return;
			}
		}
		void MapHolder::sendItemExpired(unsigned int item_id)
		{
			items_dropped.erase(item_id);
			m_Size = construct_server_packet(m_Buffer, MSG_STC_EXPIRE_ITEM, item_id);
			sendDataToAllClients(m_Size);
		}
		void MapHolder::sendNewPlayerInMap(unsigned int new_player_id)
		{
			{
				auto msg = MsgStcNewPlayerInMap{ new_player_id , {} };
				ReduceEquipsData(msg.player_equips, players_equips[new_player_id]);
				auto [data, size] = srl_dynamic_type(msg);
				m_Size = construct_server_packet_with_buffer(m_Buffer,
					MSG_STC_ADD_PLAYER, *data, size);
				sendDataToAllClients(m_Size);
			}
			{
				for (const auto [old_player_id, old_player_data] : players_equips)
				{
					if (old_player_id != new_player_id)
					{
						auto msg = MsgStcNewPlayerInMap{ old_player_id , {} };
						ReduceEquipsData(msg.player_equips, old_player_data);

						auto [data, size] = srl_dynamic_type(msg);
						m_Size = construct_server_packet_with_buffer(m_Buffer,
							MSG_STC_ADD_PLAYER, *data, size);
						m_Socket.Send(m_ClientAddress[new_player_id], m_Buffer, m_Size);
					}
				}
			}
		}
		void MapHolder::sendMobsData(Address sender)
		{
			mob_manager.recalculateAllMobs();
			auto [data, size] = srl_dynamic_type(mob_manager.m_Monsters);
			m_Size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_MOB_DATA, *data, size);
			m_Socket.Send(sender, m_Buffer, m_Size);
		}
		void MapHolder::sendMobsUpdate(unsigned int mob_id, MobMoveCommand mmc)
		{
			printf("mob %d is commanded to %d for %f seconds\n", mob_id, mmc.dir, mmc.duration);
			MonsterStateUpdate& state_data = mob_manager.m_Monsters[mob_id];
			mob_manager.setNewMoveCommand(mob_id, mmc);
			const auto size = construct_server_packet(m_Buffer, MSG_STC_MOB_UPDATE, mob_id, state_data);
			sendDataToAllClients(size);
		}
		void MapHolder::updateMobManager()
		{
			auto mob_ids = mob_manager.update();
			for (const auto& id : mob_ids)
				sendMobsUpdate(id, mob_manager.m_MoveCmds[id]);
		}
		void MapHolder::sendUpdateLocationToAll(Address sender)
		{
			RegularMapUpdate map_update_data{ players_state };
			const auto client_id = dsrl_type<unsigned int>(m_Buffer + 1);
			if (client_id == 1)
				map_update_data.m_PlayersLocation.erase(client_id); // erase this line to research delay
			else
				map_update_data.m_PlayersLocation.erase(client_id); // erase this line to research delay
			auto [data, size] = srl_dynamic_type(map_update_data);
			const auto buffer_size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_PLAYERS_LOCATIONS, *data, size);
			m_Socket.Send(sender, m_Buffer, buffer_size);
		}
		void MapHolder::sendPlayerLeftMap(unsigned int player_id)
		{
			m_Size = construct_server_packet(m_Buffer,
				MSG_STC_PLAYER_LEFT, PlayerLeftMsg{ player_id });
			for (const auto& [player_in_map, _ir] : players_state)
				m_Socket.Send(m_ClientAddress[player_in_map],
					m_Buffer, m_Size);
		}
		void MapHolder::sendWearUpdate(IdType player_id, 
			decltype(PlayerData::equips_char) char_equips)
		{
			auto msg = MsgStcNewPlayerInMap{ player_id , {} };
			ReduceEquipsData(msg.player_equips, players_equips[player_id]);
			auto [data, size] = srl_dynamic_type(msg);
			m_Size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_UPDATE_NET_CHAR_EQUIPS, *data, size);
			sendDataToAllClientsInMapExcept(m_Size, player_id);
		}
		void MapHolder::sendDataToAllClients(unsigned int size)
		{
			for (const auto& [player_id, _ir] : players_state)
				m_Socket.Send(m_ClientAddress[player_id], m_Buffer, size);
		}
		void MapHolder::sendDataToAllClientsInMapExcept(unsigned int size, unsigned int exclude_id)
		{
			for (const auto& [player_id, _ir] : players_state)
				if (player_id != exclude_id)
					m_Socket.Send(m_ClientAddress[player_id], m_Buffer, size);
		}
	}
}
