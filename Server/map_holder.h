#pragma once
#include <queue>
#include "srl/map_data.h"
#include "mob_manager.h"
#include "net/client_funcs.h"
#include "net/net_msgs.h"
#include "socket_handler.h"

namespace hiraeth {
	namespace network {
		struct ItemExpirer
		{
			unsigned int item_id;
			//float expire_time;
			ATimer expire_timer;
		};

		struct MapHolder
		{
			std::map<unsigned int, ItemDropMsg> items_dropped{};
			std::map<unsigned int, PlayerStateUpdateMsg> players_state{};
			MobManager mob_manager{ 0 };
		private:
			//Socket m_Socket;
			std::queue<ItemExpirer> m_ExpiringQueue{};
			unsigned int m_ItemsIdCounter{ 0 };

		public:
			ItemDropMsg addItem(unsigned int item_type_id, unsigned int item_kind, maths::vec2 pos)
			{
				auto new_item = ItemDropMsg{ m_ItemsIdCounter ,item_type_id, item_kind, pos };
				items_dropped[m_ItemsIdCounter] = new_item;
				m_ExpiringQueue.push(ItemExpirer{ m_ItemsIdCounter, ATimer{10.0f } });
				m_ItemsIdCounter++;
				return new_item;
			}
			void findExpiredItems()
			{
				while (!m_ExpiringQueue.empty())
				{
					if (m_ExpiringQueue.front().expire_timer.hasExpired())
					{
						if (items_dropped.find(m_ExpiringQueue.front().item_id) != items_dropped.end())
							sendItemExpired(m_ExpiringQueue.front().item_id);
						m_ExpiringQueue.pop();
					}
					else
						return;
				}
			}
			void sendItemExpired(unsigned int item_id)
			{
				items_dropped.erase(item_id);
				SocketHandler::m_Size = create_client_packet_with_data(SocketHandler::m_Buffer, MSG_STC_EXPIRE_ITEM, item_id);
				sendDataToAllClients(SocketHandler::m_Size);
			}
			void sendDataToAllClients(unsigned int size)
			{
				for (const auto& [player_id, _ir] : players_state)
					SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[player_id], SocketHandler::m_Buffer, size);
			}
			void sendNewPlayerInMap(unsigned int new_char_index)
			{
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_ADD_PLAYER, new_char_index);
				sendDataToAllClients(SocketHandler::m_Size);
			}
			void sendMobsData(Address sender)
			{
				mob_manager.recalculateAllMobs();
				auto [data, size] = srl_dynamic_type(mob_manager.m_Monsters);
				const auto buffer_size = construct_server_packet_with_buffer(SocketHandler::m_Buffer,
					MSG_STC_MOB_DATA, *data, size);
				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, buffer_size);
			}

			void sendMobsUpdate(unsigned int mob_id, MobMoveCommand mmc)
			{
				printf("mob %d is commanded to %d for %f seconds\n", mob_id, mmc.dir, mmc.duration);
				MonsterStateUpdate& state_data = mob_manager.m_Monsters[mob_id];
				mob_manager.setNewMoveCommand(mob_id, mmc);
				const auto size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_MOB_UPDATE, mob_id, state_data);
				sendDataToAllClients(size);
			}

			void updateMobManager()
			{
				auto mob_ids = mob_manager.update();
				for (const auto& id : mob_ids)
					sendMobsUpdate(id, mob_manager.m_MoveCmds[id]);
			}
			void sendUpdateLocationToAll(Address sender)
			{
				RegularMapUpdate map_update_data{ players_state };
				const auto client_id = dsrl_type<unsigned int>(SocketHandler::m_Buffer + 1);
				if (client_id == 1)
					map_update_data.m_PlayersLocation.erase(client_id); // erase this line to research delay
				else
					map_update_data.m_PlayersLocation.erase(client_id); // erase this line to research delay
				auto [data, size] = srl_dynamic_type(map_update_data);
				const auto buffer_size = construct_server_packet_with_buffer(SocketHandler::m_Buffer,
					MSG_STC_PLAYERS_LOCATIONS, *data, size);
				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, buffer_size);
			}
			void sendPlayerLeftMap(unsigned int player_id)
			{
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer,
					MSG_STC_PLAYER_LEFT, PlayerLeftMsg{ player_id });
				for (const auto& [player_in_map, _ir] : players_state)
					SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[player_in_map],
						SocketHandler::m_Buffer, SocketHandler::m_Size);
			}
		};
	}
}
