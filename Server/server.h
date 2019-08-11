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


const int MaxClients = 64;
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {

		struct Summoner
		{
			SRL::Summon summon;
			float summonTime;
		};
		//struct PlayerQuestData
		//{
		//	unsigned int quest_id, quest_stage;
		//};
		class Server
		{
		private:
			int m_maxClients;
			int m_numConnectedClients;
			std::map<char, std::function<void(Address)>> m_DistTable{};
			std::map<char, void(Server::*)(Address)> m_DistTable2{};
			//bool m_ClientConnected[MaxClients];
			Address m_ClientAddress[MaxClients];
			Socket m_Socket;
			std::vector<unsigned int> m_ClientsIds;
			// map_state
			std::map<unsigned int, PlayerStateUpdateMsg> m_ClientsState;
			std::queue<Summoner> m_SummonQueue;
			MobManager m_MobManager;
			//players_states
			//std::map<unsigned int, std::vector<PlayerQuestData>> m_PlayerQuestData;
			std::map<unsigned int, std::map<unsigned int, unsigned int>> m_PlayerQuestData;

			BufferType m_Buffer[256];
			size_t m_Size{0};
			ATimer m_Timer;

			struct QueueData
			{
				char * buffer{nullptr};
				int size{0};
				Address sender{};
			};
			std::queue<QueueData> m_DataQueue;
			std::mutex m_Mutex;
			std::condition_variable m_Cv;
		public:
			Server()
				: m_maxClients(MaxClients),
				m_numConnectedClients(0),
				//m_ClientConnected{ false },
				m_MobManager{ 0 }
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
				bindFunctionToChar(MSG_INR_MOB_HIT, &Server::InrMobGotHit);
				bindFunctionToChar(MSG_INR_MOB_UPDATE, &Server::InrMobUpdate);
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

			void bindFunctionToChar(char bytecode, void(Server::*fptr)(Address) )
			{
				std::function<void(Address)> val = std::bind(fptr, this, std::placeholders::_1);
				std::function<void(Address)> val2 = [&](Address sender) {(this->*fptr)(sender); };
				m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val));
				m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val2));
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
				auto [client_id, monster_damage] = dsrl_types<unsigned int, MonsterDamage>(m_Buffer + 1);
				//const auto monster_damage = dsrl_types<MonsterDamage>(m_Buffer + 5);
				{ // send hit update to all players (except for the one attacked)
					//const auto client_id = dsrl_types<unsigned int>(m_Buffer + 1);
					const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_MOB_HIT, monster_damage);
					sendDataToAllClientsExcept(buffer_size, client_id);
				}
				// update mob hp
				if (m_MobManager.damageMob(monster_damage))
				{
					m_MobManager.monsterDied(monster_damage.monster_id);
					//std::vector<unsigned int> dropped_items {0};
					auto[data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_damage.monster_id, {0} });
					const auto buffer_size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_MOB_DIED, *data, size);
					sendDataToAllClients(buffer_size);
				}
				//updateMonstersHp()
			}
			void NpcClick(Address sender)
			{
				auto [client_id, npc_index] = dsrl_types<unsigned int, unsigned int>(m_Buffer + 1);
				unsigned int dialog_id = 0 ;
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_START_DIALOG, dialog_id);
				m_Socket.Send(sender, m_Buffer, buffer_size);
			}
			void DialogNext(Address sender)
			{
				auto [client_id, npc_index, dialog_index] = dsrl_types<unsigned int, unsigned int, unsigned int>(m_Buffer + 1);
				//unsigned int client_id, npc_index, dialog_index;
				//dsrl_types(m_Buffer, client_id, npc_index, dialog_index);
				const auto buffer_size = construct_server_packet(m_Buffer, MSG_STC_DIALOG_NEXT_ANSWER, dialog_index + 1);
				m_Socket.Send(sender, m_Buffer, buffer_size);
			}
			void AcceptQuest(Address sender)
			{
				auto [client_id, npc_index, dialog_index] = dsrl_types<unsigned int, unsigned int, unsigned int>(m_Buffer + 1);
				if (m_PlayerQuestData.find(client_id) == m_PlayerQuestData.end())
					m_PlayerQuestData[client_id] = std::map<unsigned int, unsigned int>();
				if (m_PlayerQuestData[client_id].find(npc_index) == m_PlayerQuestData[client_id].end())
					m_PlayerQuestData[client_id][npc_index] = 0;
				m_PlayerQuestData[client_id][npc_index] += 1;
			}
			void InrMobGotHit(Address sender)
			{
			}
			void InrMobUpdate(Address sender)
			{
				updateMobManager();
				createMessageThread(MSG_INR_MOB_UPDATE, 1000);
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
				for (int i = 0; i < m_maxClients; ++i)
				{
					//if (!m_ClientConnected[i])
					if (!isClientConnected(i))
						return i;
				}
				return -1;
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