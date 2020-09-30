#pragma once
#include<cstdio>
#include<winsock2.h>
 
#define _WIN32_WINNT _WIN32_WINNT_WIN8 // Windows 8.0
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {

		class Handler
		{
		private:
			SOCKET m_Socket{};
		public:
			Handler(SOCKET s)
				: m_Socket(s)
			{
				
			}

			void update()
			{
				
			}
		};
	}
}