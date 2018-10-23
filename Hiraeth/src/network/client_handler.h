#pragma once
#include<cstdio>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include "basic/a_timer.h"
#include "net/protocol.h"
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

			struct sockaddr_in si_other {};
			int s, slen = sizeof(si_other);
			char buf[BUFLEN];
			char message[BUFLEN];
			WSADATA wsa;
			ATimer m_KATimer{ 1000.0 };
		public:
			ClientHandler()
			{
				if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
				{
					printf("Failed. Error Code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
				{
					printf("socket() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}

				//setup address structure
				memset(reinterpret_cast<char *>(&si_other), 0, sizeof(si_other));
				si_other.sin_family = AF_INET;
				si_other.sin_port = htons(PORT);
				InetPton(AF_INET, _T(SERVER), &si_other.sin_addr.S_un.S_addr);

				strcpy(message, "hello");
				if (sendto(s, message, strlen(message), 0, reinterpret_cast<struct sockaddr *>(&si_other), slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
			}

			void Update()
			{
				if (m_KATimer.hasExpired())
				{
					strcpy_s(message, "hello");
					if (sendto(s, message, strlen(message), 0, reinterpret_cast<struct sockaddr *>(&si_other), slen) == SOCKET_ERROR)
					{
						printf("sendto() failed with error code : %d", WSAGetLastError());
						exit(EXIT_FAILURE);
					}
					m_KATimer.reSet(1000);
				}
			}

		};
	}
}