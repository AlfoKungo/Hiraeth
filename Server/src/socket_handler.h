#pragma once
#include "socket.h"
#include "net/protocol.h"

const int MaxClients = 64;

namespace hiraeth {
	namespace network {

		extern Socket m_Socket;
		extern Address m_ClientAddress[MaxClients];
		extern BufferType m_Buffer[BufferSize];
		extern size_t m_Size;
	//	class SocketHandler
	//	{
	//	public:
	//		static Socket m_Socket;
	//		static Address m_ClientAddress[MaxClients];
	//		static BufferType m_Buffer[BufferSize];
	//		static size_t m_Size;
	//	};
	}
}
