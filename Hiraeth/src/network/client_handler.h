#pragma once
#include<cstdio>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include "basic/a_timer.h"
#include "net/protocol.h"
#include "maths/vec2.h"
#include <map>
#include <cereal/archives/binary.hpp>
#include <iostream>
#include <fstream>
#include "game/net_chars/net_char_manager.h"
//#include "net/protocol.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define SERVER "127.0.0.1"  //ip address of udp server
const int BUFLEN = 512;  //Max length of buffer
const int PORT = 8888;   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {

		class ClientHandler
		{
		private:
			const float KA_TIMEOUT = 1.0f, KA_LOSS_TIMEOUT = 3.0f;

			SOCKET m_Handle;
			unsigned int m_Id{0};
			struct sockaddr_in m_SiOther {};
			int slen = sizeof(m_SiOther), m_SendSize;
			char m_RcvBuffer[BUFLEN], m_SendBuffer[BUFLEN];
			ATimer m_KATimer{ KA_TIMEOUT }, m_KALossTimer{ KA_LOSS_TIMEOUT };

			game::NetCharManager * m_NetCharManager;
			std::map<unsigned int, maths::vec2> m_PlayerLocation;
		public:
			ClientHandler(game::NetCharManager * net_char_manager)
				: m_RcvBuffer{0}, m_SendBuffer{0},
			m_NetCharManager(net_char_manager)
			{
				WSADATA wsa;
				if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
				{
					printf("Failed. Error Code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				if ((m_Handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
				{
					printf("socket() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}

				//setup address structure
				memset(reinterpret_cast<char *>(&m_SiOther), 0, sizeof(m_SiOther));
				m_SiOther.sin_family = AF_INET;
				m_SiOther.sin_port = htons(PORT);
				InetPton(AF_INET, _T(SERVER), &m_SiOther.sin_addr.S_un.S_addr);

				const char message[] = "hello";
				m_SendSize = construct_client_packet(m_SendBuffer, MSG_CTS_OPEN_CONNECTION, m_Id, message, sizeof(message));
				if (sendto(m_Handle, m_SendBuffer, strlen(m_SendBuffer), 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				int recv_len;
				if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), &slen)) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != 10035)
						printf("recvfrom() failed with error code : %d", WSAGetLastError());
				}
				if (recv_len == 4)
				{
					memcpy(&m_Id, m_RcvBuffer, sizeof(unsigned int));
					printf("received Id number of : %d\n", m_Id);
				}

				DWORD nonBlocking = 1;
				if (ioctlsocket(m_Handle, FIONBIO, &nonBlocking) != 0)
				{
					printf("failed to set non-blocking\n");
				}
			}

			~ClientHandler()
			{
				closeConnection();
			}

			void closeConnection()
			{
				const char message[] = "hello";
				m_SendSize = construct_client_packet(m_SendBuffer, MSG_CTS_CLOSE_CONNECTION, m_Id, message, sizeof(message));
				if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d\n", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				printf("Sent keep_alive\n");
			}

			void Update(maths::vec2 char_pos)
			{
				handleKeepAlive();
				sendLocationUpdate(char_pos);
				receiveData();
			}

			void sendLocationUpdate(maths::vec2 char_pos)
			{
				char message[sizeof(maths::vec2)];
				memcpy(message, &char_pos, sizeof(maths::vec2));
				m_SendSize = construct_client_packet(m_SendBuffer, MSG_CTS_LOCATION_UPDATE, m_Id, message, sizeof(message));
				if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d\n", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
			}

			void handleKeepAlive()
			{
				if (m_KATimer.hasExpired())
				{
					const char message[] = "hello";
					m_SendSize = construct_client_packet(m_SendBuffer, MSG_CTS_KA, m_Id, message, sizeof(message));
					if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
					{
						printf("sendto() failed with error code : %d\n", WSAGetLastError());
						exit(EXIT_FAILURE);
					}
					printf("Sent keep_alive\n");
					m_KATimer.reSet(KA_TIMEOUT);
				}
				memset(m_RcvBuffer,'\0', BUFLEN);
				if (m_KALossTimer.hasExpired())
				{
					printf("I lost keep_alive\n");
					m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
				}
			}

			void receiveData()
			{
				int recv_len;
				if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), &slen)) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != 10035)
						printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
				}
				if (recv_len > 0)
				{
					switch (m_RcvBuffer[0])
					{
					case MSG_STC_ADD_PLAYER:
						addNewPlayerToMap(m_RcvBuffer);
						break;
					case MSG_STC_PLAYERS_LOCATIONS:
						updatePlayersLocation();
						break;
					case MSG_STC_PLAYERS_LIST:
						loadCurrentMapPlayers(m_RcvBuffer);
						break;
					default:
						break;
					}
					//if (strcmp(m_RcvBuffer, "ack") == 0)
					//{
					//	printf("Received keep_alive \n");
					//	m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
					//}
				}
			}

			void addNewPlayerToMap(char * buffer)
			{
				const auto new_char_id = dsrl_packet_data<unsigned int>(buffer + 1);
				m_NetCharManager->addChar(new_char_id, maths::vec2{ 0,0 });
			}

			void updatePlayersLocation()
			{
				dsrl_dt_packet_data(m_PlayerLocation, m_RcvBuffer + 1);
				m_PlayerLocation.erase(m_Id);
				for (const auto& player : m_PlayerLocation)
					m_NetCharManager->updateCharPos(player.first, player.second);
				m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
			}

			void loadCurrentMapPlayers(char * buffer)
			{
				//dsrl_dt_packet_data(m_PlayerLocation, m_RcvBuffer + 1);
				//m_PlayerLocation.erase(m_Id);
				//for (const auto& player : m_PlayerLocation)
				//	m_NetCharManager->updateCharPos(player.first, player.second);
				//m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
			}

			//unsigned int constructMsg(unsigned char msgId, const char * msg, size_t message_len)
			//{
			//	memset(m_SendBuffer,'\0', BUFLEN);
			//	memcpy(m_SendBuffer, &msgId, sizeof(char));
			//	memcpy(m_SendBuffer + 1, &m_Id, sizeof(unsigned int));
			//	memcpy(m_SendBuffer + 5, msg, message_len);
			//	return 5 + message_len;
			//}

		};
	}
}