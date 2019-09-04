#pragma once
#include <queue>
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <bitset>
#include <map>
#include "address.h"
#include "socket.h"
#include "net/protocol.h"
#include "net/srl_funcs.h"
#include "net/server_funcs.h"
#include "maths/vec2.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/map.hpp"
#include "utills/a_timer.h"
#include "srl/map_data.h"
#include "mob_manager.h"
#include "db_client.h"
#include "net/client_funcs.h"


const int MaxClients = 64;
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {

		//struct Summoner
		//{
		//	SRL::Summon summon;
		//	float summonTime;
		//};
		struct ItemExpirer
		{
			unsigned int item_id;
			//float expire_time;
			ATimer expire_timer;
		};
		//struct PlayerQuestData
		//{
		//	unsigned int quest_id, quest_stage;
		//};
		class Server
		{
		private:
			int m_maxClients;
			int m_NumConnectedClients;
			std::map<char, std::function<void(Address)>> m_DistTable{};
			std::map<char, void(Server::*)(Address)> m_DistTable2{};
			//bool m_ClientConnected[MaxClients];
			Address m_ClientAddress[MaxClients];
			Socket m_Socket;
			std::vector<unsigned int> m_ClientsIds;
			// map_state
			std::map<unsigned int, PlayerStateUpdateMsg> m_ClientsState;
			//std::queue<Summoner> m_SummonQueue;
			MobManager m_MobManager;
			std::map<unsigned int, ItemDropMsg> m_ItemsDropped;
			std::queue<ItemExpirer> m_ExpiringQueue;
			unsigned int m_ItemsIdCounter{};
			//players_states
			//std::map<unsigned int, std::vector<PlayerQuestData>> m_PlayerQuestData;
			std::map<unsigned int, std::map<unsigned int, unsigned int>> m_PlayerQuestData;
			DbClient * m_DbClient;

			BufferType m_Buffer[512];
			size_t m_Size{ 0 };
			ATimer m_Timer;

			struct QueueData
			{
				char * buffer{ nullptr };
				int size{ 0 };
				Address sender{};
			};
			std::queue<QueueData> m_DataQueue;
			std::mutex m_Mutex;
			std::condition_variable m_Cv;
		public:
			Server(DbClient * db_client)
				: m_maxClients(MaxClients),
				m_NumConnectedClients(0),
				//m_ClientConnected{ false },
				m_MobManager{ 0 },
				m_DbClient(db_client)
			{
				if (!m_Socket.Open(PORT))
				{
					printf("failed to create socket!\n");
					return;
				}
				bindFunctionToChar(MSG_CTS_OPEN_CONNECTION, &Server::OpenConnection);
				bindFunctionToChar(MSG_CTS_CLOSE_CONNECTION, &Server::CloseConnection);
				bindFunctionToChar(MSG_CTS_LOCATION_UPDATE, &Server::LocationUpdate);
				bindFunctionToChar(MSG_CTS_KA, &Server::KeepAlive);
				bindFunctionToChar(MSG_CTS_HIT_MOB, &Server::HitMob);
				bindFunctionToChar(MSG_CTS_NPC_CLICK, &Server::NpcClick);
				bindFunctionToChar(MSG_CTS_DIALOG_NEXT, &Server::DialogNext);
				bindFunctionToChar(MSG_CTS_ACCEPT_QUEST, &Server::AcceptQuest);
				bindFunctionToChar(MSG_CTS_CHAR_GOT_HIT, &Server::CharGotHit);
				bindFunctionToChar(MSG_CTS_CHAR_USE_SKILL_E, &Server::CharUseSkillE);
				bindFunctionToChar(MSG_CTS_CHAR_USE_SKILL_A, &Server::CharUseSkillA);
				bindFunctionToChar(MSG_CTS_PICK_ITEM, &Server::PickItem);
				bindFunctionToChar(MSG_CTS_INCREASE_SKILL, &Server::IncreaseSkill);
				bindFunctionToChar(MSG_INR_MOB_HIT, &Server::InrMobGotHit);
				bindFunctionToChar(MSG_INR_MOB_UPDATE, &Server::InrMobUpdate);
				bindFunctionToChar(MSG_INR_ROUTINE_UPDATE, &Server::RoutineUpdate);

				//for (unsigned int i = 0; i < 15; ++i)
				//	sendDropItem(addItem((i %9) %5, (i/5) %2, maths::vec2(int(i - 6) *80, 0)));
			}
			void main_60fps_loop();
			void main_block_receive_and_async_send();
			void main_block_threaded_queue();
			//void dataReaderCv();
			void main_60fps_threaded_queue();
			void dataReader();

			void addMessageAfterT(int time, char * buffer, int size);
			void createMessageThread(char MessageType, int milliseconds);

			void switchData(Address sender);

			void sendConnectionResponse(Address sender);
			void sendNewPlayerInMap(unsigned int new_char_index);
			void closeConnection(BufferType* buffer);
			void receiveLocation(BufferType* buffer);
			void sendUpdateLocationToAll(Address sender);
			void sendMobsData(Address sender);
			void sendMobsUpdate(unsigned int mob_id, MobMoveCommand mmc);
			void updateMobManager();

			void bindFunctionToChar(char bytecode, void(Server::*fptr)(Address))
			{
				std::function<void(Address)> val = std::bind(fptr, this, std::placeholders::_1);
				//std::function<void(Address)> val2 = [&](Address sender) {(this->*fptr)(sender); };
				m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val));
				//m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val2));
				m_DistTable2.insert(std::pair<char, void(Server::*)(Address)>(bytecode, fptr));
			}

			// Destined Functions
			void OpenConnection(Address sender)
			{
				sendConnectionResponse(sender);
				m_MobManager.recalculateAllMobs();
				sendMobsData(sender);
			}
			void CloseConnection(Address sender)
			{
				closeConnection(m_Buffer);
			}
			void LocationUpdate(Address sender)
			{
				receiveLocation(m_Buffer);
				sendUpdateLocationToAll(sender);
			}
			void KeepAlive(Address sender)
			{
				sendUpdateLocationToAll(sender);
			}
			void HitMob(Address sender)
			{
				//auto[client_id, monster_damage] = dsrl_types<unsigned int, MonsterDamage>(m_Buffer + 1);
				auto[client_id, monster_hit] = dsrl_types<unsigned int, MonsterHit>(m_Buffer + 1);
				//const auto monster_damage = dsrl_types<MonsterDamage>(m_Buffer + 5);
				{ // send hit update to all players (except for the one attacked)
					//const auto client_id = dsrl_types<unsigned int>(m_Buffer + 1);
					const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_MOB_HIT, monster_hit);
					sendDataToAllClientsExcept(buffer_size, client_id);
				}
				// update mob hp
				if (m_MobManager.damageMob(monster_hit))
				{
					auto dead_pos = m_MobManager.monsterDied(monster_hit.monster_id);
					//std::vector<unsigned int> dropped_items {0};
					//auto[data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_damage.monster_id, {0} });
					//m_ItemsDropped[m_ItemsIdCounter] = ItemDropMsg{ m_ItemsIdCounter ,0, 0, dead_pos };
					//auto[data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_hit.monster_id, {m_ItemsDropped[m_ItemsIdCounter] } });
					//m_ItemsIdCounter++;
					auto[data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_hit.monster_id, {addItem(0, 0, dead_pos)} });
					const auto buffer_size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_MOB_DIED, *data, size);
					sendDataToAllClients(buffer_size);
				}
				//updateMonstersHp()
			}
			ItemDropMsg addItem(unsigned int item_type_id, unsigned int item_kind, maths::vec2 pos)
			{
				auto new_item = ItemDropMsg{ m_ItemsIdCounter ,item_type_id, item_kind, pos };
				m_ItemsDropped[m_ItemsIdCounter] = new_item;
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
						if (m_ItemsDropped.find(m_ExpiringQueue.front().item_id) != m_ItemsDropped.end())
							sendItemExpired(m_ExpiringQueue.front().item_id);
						m_ExpiringQueue.pop();
					}
					else
						return;
				}
			}
			void NpcClick(Address sender)
			{
				auto[client_id, npc_index] = dsrl_types<unsigned int, unsigned int>(m_Buffer + 1);
				unsigned int dialog_id = 0;
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_START_DIALOG, dialog_id);
				m_Socket.Send(sender, m_Buffer, buffer_size);
			}
			void DialogNext(Address sender)
			{
				auto[client_id, npc_index, dialog_index] = dsrl_types<unsigned int, unsigned int, unsigned int>(m_Buffer + 1);
				//unsigned int client_id, npc_index, dialog_index;
				//dsrl_types(m_Buffer, client_id, npc_index, dialog_index);
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_DIALOG_NEXT_ANSWER, dialog_index + 1);
				m_Socket.Send(sender, m_Buffer, buffer_size);
			}
			void AcceptQuest(Address sender)
			{
				auto[client_id, npc_index, dialog_index] = dsrl_types<unsigned int, unsigned int, unsigned int>(m_Buffer + 1);
				if (m_PlayerQuestData.find(client_id) == m_PlayerQuestData.end())
					m_PlayerQuestData[client_id] = std::map<unsigned int, unsigned int>();
				if (m_PlayerQuestData[client_id].find(npc_index) == m_PlayerQuestData[client_id].end())
					m_PlayerQuestData[client_id][npc_index] = 0;
				m_PlayerQuestData[client_id][npc_index] += 1;
			}
			void CharGotHit(Address sender)
			{
				auto[client_id, new_hp] = dsrl_types<unsigned int, unsigned int>(m_Buffer + 1);
				m_DbClient->setValue(client_id, "hp", new_hp);
			}
			void CharUseSkillE(Address sender)
			{
				auto[client_id, skill_id, new_mp] = dsrl_types<unsigned int, unsigned int, unsigned int>(m_Buffer + 1);
				m_DbClient->setValue(client_id, "mp", new_mp);
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_CHAR_USE_SKILL_E, client_id ,skill_id);
				sendDataToAllClientsExcept(buffer_size, client_id);
			}
			void CharUseSkillA(Address sender)
			{
				auto[client_id] = dsrl_types<unsigned int>(m_Buffer + 1);
				const auto attack_skill_msg = dsrl_dynamic_type<AttackSkillMsg>(m_Buffer + 5);
				CharAttackSkillMsg char_attack_msg{ client_id, attack_skill_msg };
				auto[data, size] = srl_dynamic_type(char_attack_msg);
				const auto buffer_size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_CHAR_USE_SKILL_A, *data, size);
				sendDataToAllClientsExcept(buffer_size, client_id);

				for (const auto& monster_hit : attack_skill_msg.monsters_hit)
				if (m_MobManager.damageMob(monster_hit))
				{
					auto dead_pos = m_MobManager.monsterDied(monster_hit.monster_id);
					auto[data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_hit.monster_id, {addItem(0, 0, dead_pos)} });
					const auto buffer_size2 = construct_server_packet_with_buffer(m_Buffer, MSG_STC_MOB_DIED, *data, size);
					sendDataToAllClients(buffer_size2);
				}
				//sendDataToAllClients(buffer_size);
			}
			void PickItem(Address sender)
			{
				auto[client_id, item_id] = dsrl_types<unsigned int, unsigned int>(m_Buffer + 1);
				//const auto buffer_size = construct_server_packet(m_Buffer, MSC_STC_CHAR_USE_SKILL_A, client_id, item_id);
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_PICK_ITEM, PickItemMsg{ client_id, item_id });
				m_ItemsDropped.erase(item_id);
				sendDataToAllClientsExcept(buffer_size, client_id);
				//sendDataToAllClients(buffer_size);
			}
			void IncreaseSkill(Address sender)
			{
				auto[client_id, skill_id] = dsrl_types<unsigned int, unsigned int>(m_Buffer + 1);
				m_DbClient->increaseSkillPoints(client_id, skill_id);
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_INCREASE_SKILL,  skill_id);
				m_Socket.Send(sender, m_Buffer, buffer_size);
			}
			void InrMobGotHit(Address sender)
			{
			}
			void InrMobUpdate(Address sender)
			{
				updateMobManager();
				createMessageThread(MSG_INR_MOB_UPDATE, 1000);
			}
			void sendDropItem(ItemDropMsg item_drop)
			{
				//m_ItemsDropped[item_drop.item_id] = item_drop;
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_DROP_ITEM, item_drop);

				sendDataToAllClients(buffer_size);
			}
			void sendDroppedItems(Address sender)
			{
				std::vector<ItemDropMsg> dropped_items;
				for (auto di : m_ItemsDropped)
					dropped_items.push_back(di.second);
				auto[data, size] = srl_dynamic_type(dropped_items);
				const auto buffer_size = construct_server_packet_with_buffer(m_Buffer,
					MSG_STC_DROPPED_ITEM, *data, size);
				m_Socket.Send(sender, m_Buffer, buffer_size);
			}
			void sendItemExpired(unsigned int item_id)
			{
				m_ItemsDropped.erase(item_id);
				const auto buffer_size = create_client_packet_with_data(m_Buffer, MSG_STC_EXPIRE_ITEM, item_id);
				sendDataToAllClients(buffer_size);
			}
			void RoutineUpdate(Address sender)
			{
				findExpiredItems();
				createMessageThread(MSG_INR_ROUTINE_UPDATE, 1000);
			}

			//void sendKeepAliveAnswer(Address sender)
			//{
			//	printf("ip is : %d %s %s and port is : %d\n", sender.GetAddress(),
			//		std::bitset<32>(sender.GetAddress()).to_string().c_str(),
			//		sender.GetAddressString().c_str(), sender.GetPort());
			//	const char data[] = "ack";
			//	m_Socket.Send(sender, data, sizeof(data));
			//}

			int findFreeClientIndex() const
			{
				return 2;
				//for (int i = 0; i < m_maxClients; ++i)
				//{
				//	//if (!m_ClientConnected[i])
				//	if (!isClientConnected(i))
				//		return i;
				//}
				//return -1;
			}
			unsigned int findExistingClientIndex(const Address & address) const
			{
				for (int i = 0; i < m_maxClients; ++i)
				{
					//if (m_clientConnected[i] && m_clientAddress[i] == address)
					if (m_ClientAddress[i].GetAddress() == address.GetAddress()
						&& m_ClientAddress[i].GetPort() == address.GetPort())
						return i;
				}
				return -1;
			}
			void sendDataToAllClients(unsigned int size)
			{
				//for (const auto& client : m_ClientAddress)
				//	if (client.GetAddress() != 0)
				//		m_Socket.Send(client, m_Buffer, size);
				for (const auto& client_id : m_ClientsIds)
					m_Socket.Send(m_ClientAddress[client_id], m_Buffer, size);
			}
			void sendDataToAllClientsExcept(unsigned int size, unsigned int exclude_id)
			{
				//const auto client_id_to_exclude = findExistingClientIndex(exclude_address);
				//for (const auto& client_id : m_ClientsIds)
				//	if (client_id != client_id_to_exclude)
				//		m_Socket.Send(m_ClientAddress[client_id], m_Buffer, size);
				for (const auto& client_id : m_ClientsIds)
					if (client_id != exclude_id)
						m_Socket.Send(m_ClientAddress[client_id], m_Buffer, size);
			}
			bool isClientConnected(int client_id) const
			{
				//return m_ClientConnected[clientIndex];
				return std::find(m_ClientsIds.begin(), m_ClientsIds.end(), client_id) != m_ClientsIds.end();
			}
			const Address & GetClientAddress(int clientIndex) const
			{
				return m_ClientAddress[clientIndex];
			}
		};
	}
}