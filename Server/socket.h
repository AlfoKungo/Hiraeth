#pragma once
#include "address.h"
#include<cstdio>
#include<winsock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#define _WIN32_WINNT _WIN32_WINNT_WIN8 // Windows 8.0
#pragma comment(lib,"ws2_32.lib") //Winsock Library

namespace hiraeth {
	namespace network {

#define BUFLEN 512  //Max length of buffer

		class Socket
		{
		private:

		public:
			unsigned int handle;
			Socket();

			~Socket();

			bool Open(unsigned short port);

			void SetUnblocking() const;

			void Close();

			bool IsOpen() const;

			bool Send(const Address & destination,
				const void * data,
				int size);

			int Receive(Address & sender,
				void * data,
				int size);

		};
	}
}
