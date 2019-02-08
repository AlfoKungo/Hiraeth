#pragma once
#include "address.h"
#include "socket.h"
#include <bitset>
#include "net/protocol.h"
#include "net/server_funcs.h"
#include "maths/vec2.h"
#include <map>
#include "cereal/archives/binary.hpp"
#include "cereal/types/map.hpp"
#include "utills/a_timer.h"
#include "srl/map_data.h"
#include <queue>
#include <chrono>
#include <thread>
#include <future>
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

		class Server
		{
		private:
			int m_maxClients;
			int m_numConnectedClients;
			//bool m_ClientConnected[MaxClients];
			Address m_ClientAddress[MaxClients];
			Socket m_Socket;
			std::vector<unsigned int> m_ClientsIds;
			// map_state
			std::map<unsigned int, PlayerStateUpdate> m_ClientsState;
			std::queue<Summoner> m_SummonQueue;
			MobManager m_MobManager;

			BufferType m_Buffer[256];
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
			void sendMobGotHit(unsigned int mob_id, Direction dir);

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
			int findExistingClientIndex(const Address & address) const
			{
				for (int i = 0; i < m_maxClients; ++i)
				{
					//if (m_clientConnected[i] && m_clientAddress[i] == address)
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