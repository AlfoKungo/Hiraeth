#pragma once
#include "socket.h"
#include "net/protocol.h"

const int MaxClients = 64;

namespace hiraeth {
	namespace network {

		class SocketHandler
		{
		public:
			static Socket m_Socket;
			static Address m_ClientAddress[MaxClients];
			static BufferType m_Buffer[BufferSize];
			static size_t m_Size;
		};
	}
}
