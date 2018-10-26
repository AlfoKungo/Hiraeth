#pragma once
#include "address.h"
#include "socket.h"
#include <bitset>
#include "net/protocol.h"
#include "maths/vec2.h"
#include <map>
#include "cereal/archives/binary.hpp"
#include "cereal/types/map.hpp"


const int MaxClients = 64;
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {

		class Server
		{
		private:
			int m_maxClients;
			int m_numConnectedClients;
			bool m_ClientConnected[MaxClients];
			Address m_ClientAddress[MaxClients];
			Socket m_Socket;
			char m_Buffer[256];
			std::vector<unsigned int> m_ClientsIds;
			std::map<unsigned int, maths::vec2> m_ClientsPos;
		public:
			Server()
				: m_maxClients(MaxClients),
				m_numConnectedClients(0),
				m_ClientConnected{ false }
			{

			}
			void main_function()
			{
				m_ClientsPos[1] = maths::vec2{ 20,-34 };
				m_ClientsPos[2] = maths::vec2{ 56,12 };
				if (!m_Socket.Open(PORT))
				{
					printf("failed to create socket!\n");
					return;
				}
				while (true)
				{
					Address sender;
					const int bytes_read =
						m_Socket.Receive(sender, m_Buffer, sizeof(m_Buffer));
					if (bytes_read > 0)
						switch (m_Buffer[0])
						{
						case MSG_CTS_OPEN_CONNECTION:
							sendConnectionResponse(sender);
							break;
						case MSG_CTS_CLOSE_CONNECTION:
							closeConnection(m_Buffer);
							break;
						case MSG_CTS_LOCATION_UPDATE:
							receiveLocation(m_Buffer);
							break;
						case MSG_CTS_KA:
							//sendKeepAliveAnswer(sender);
							sendUpdateLocationToAll(sender);
							break;
						default:
							break;
						}
				}
			}

			void sendConnectionResponse(Address sender)
			{
				const auto free_client_index = FindFreeClientIndex();
				m_numConnectedClients++;
				m_ClientAddress[free_client_index] = sender;
				m_ClientConnected[free_client_index] = true;
				char data[4];
				unsigned int id = free_client_index;
				memcpy(data, &id, sizeof(unsigned int));
				printf("registered new address : %s , and port is : %d , and id is %d\n",
					sender.GetAddressString().c_str(), sender.GetPort(), id);
				m_Socket.Send(sender, data, sizeof(data));
			}

			void closeConnection(char* buffer)
			{
				unsigned int id;
				memcpy(&id, buffer + 1, sizeof(unsigned int));
				m_numConnectedClients--;
				m_ClientAddress[id] = Address{};
				m_ClientConnected[id] = false;
			}

			void receiveLocation(char* buffer)
			{
				unsigned int id;
				memcpy(&id, buffer + 1, sizeof(unsigned int));
				maths::vec2 char_pos;
				memcpy(&char_pos, buffer + 5, sizeof(maths::vec2));
			}

			//void sendKeepAliveAnswer(Address sender)
			//{
			//	printf("ip is : %d %s %s and port is : %d\n", sender.GetAddress(),
			//		std::bitset<32>(sender.GetAddress()).to_string().c_str(),
			//		sender.GetAddressString().c_str(), sender.GetPort());
			//	const char data[] = "ack";
			//	m_Socket.Send(sender, data, sizeof(data));
			//}

			void sendUpdateLocationToAll(Address sender)
			{
				//char * data = new char[sizeof(maths::vec2) * m_ClientsIds.size()];
				//char data[sizeof(maths::vec2) * m_ClientsIds.size()];
				//for (const auto& client_id : m_ClientsIds)
				//{
				//	
				//}

				//std::stringstream os{ std::ios::binary | std::ios::out };
				//{
				//	cereal::BinaryOutputArchive ar(os);
				//	ar(m_ClientsPos);
				//} // the binary archives will flush their output 
				//std::string data_str = os.str();
				//auto data = new char[data_str.size() + 1];
				//data[0] = char(data_str.size());
				//memcpy(data + 1, data_str.c_str(), data_str.size());
				//m_Socket.Send(sender, data, data_str.size() + 1);
				auto [data, size] = serialize_packet_data(m_ClientsPos);
				const auto buffer_size = construct_server_packet(m_Buffer,
					MSG_STC_PLAYERS_LOCATIONS, data, size);
				m_Socket.Send(sender, m_Buffer, buffer_size);
				delete[] data;
			}

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