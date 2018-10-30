#pragma once
#include "address.h"
#include "socket.h"
#include <bitset>
#include "net/protocol.h"
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
			bool m_ClientConnected[MaxClients];
			Address m_ClientAddress[MaxClients];
			Socket m_Socket;
			std::vector<unsigned int> m_ClientsIds;
			std::map<unsigned int, PlayerStateUpdate> m_ClientsState;
			std::queue<Summoner> m_SummonQueue;

			char m_Buffer[256];
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
				m_ClientConnected{ false }
			{

			}

			void main_function();
			void altMain();
			void altMain2();
			void dataReader();
			void addMessageIn(int time, char * buffer, int size);

			void sendConnectionResponse(Address sender);
			void sendNewPlayerInMap(unsigned int new_char_index);
			void closeConnection(char* buffer);
			void receiveLocation(char* buffer);
			void sendUpdateLocationToAll(Address sender);

			//void sendKeepAliveAnswer(Address sender)
			//{
			//	printf("ip is : %d %s %s and port is : %d\n", sender.GetAddress(),
			//		std::bitset<32>(sender.GetAddress()).to_string().c_str(),
			//		sender.GetAddressString().c_str(), sender.GetPort());
			//	const char data[] = "ack";
			//	m_Socket.Send(sender, data, sizeof(data));
			//}

			//unsigned int constructMsg(unsigned char msgId, const char * msg, size_t message_len)
			//{
			//	memset(m_SendBuffer,'\0', BUFLEN);
			//	memcpy(m_SendBuffer, &msgId, sizeof(char));
			//	memcpy(m_SendBuffer + 1, &m_Id, sizeof(unsigned int));
			//	memcpy(m_SendBuffer + 5, msg, message_len);
			//	//m_SendSize = 5 + message_len;
			//	return 5 + message_len;
			//}

			int FindFreeClientIndex() const
			{
				for (int i = 0; i < m_maxClients; ++i)
				{
					if (!m_ClientConnected[i])
						return i;
				}
				return -1;
			}
			int FindExistingClientIndex(const Address & address) const
			{
				for (int i = 0; i < m_maxClients; ++i)
				{
					//if (m_clientConnected[i] && m_clientAddress[i] == address)
						return i;
				}
				return -1;
			}
			bool IsClientConnected(int clientIndex) const
			{
				return m_ClientConnected[clientIndex];
			}
			const Address & GetClientAddress(int clientIndex) const
			{
				return m_ClientAddress[clientIndex];
			}
		};
	}
}