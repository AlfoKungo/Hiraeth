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
//#include "net/protocol.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

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
		public:
			ClientHandler()
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
				//m_SendSize = constructMsg(MSG_TYPE_OPEN_CONNECTION, message, sizeof(message));
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
				//m_SendSize = constructMsg(MSG_TYPE_CLOSE_CONNECTION, message, sizeof(message));
				m_SendSize = construct_client_packet(m_SendBuffer, MSG_CTS_CLOSE_CONNECTION, m_Id, message, sizeof(message));
				if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d\n", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				printf("Sent keep_alive\n");
			}

			void Update()
			{
				handleKeepAlive();
				sendLocationUpdate({ 40,-25 });
				receiveData();
			}

			void sendLocationUpdate(maths::vec2 char_pos)
			{
				char message[sizeof(maths::vec2)];
				memcpy(message, &char_pos, sizeof(maths::vec2));
				//m_SendSize = constructMsg(MSG_TYPE_LOCATION_UPDATE, message, sizeof(message));
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
					//m_SendSize = constructMsg(MSG_TYPE_KA, message, sizeof(message));
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
					case MSG_STC_PLAYERS_LOCATIONS:
						updatePlayersLocation();
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

			void updatePlayersLocation()
			{
				std::map<unsigned int, maths::vec2> m_CharsPos;
				const std::string tmp_str{ m_RcvBuffer + 1 + 1, static_cast<unsigned int>(m_RcvBuffer[1]) };
				std::stringstream is(tmp_str, std::ios::binary | std::ios::in);
				{
					cereal::BinaryInputArchive ar(is);
					ar(m_CharsPos);
					m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
				}
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