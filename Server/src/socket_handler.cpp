#include "socket_handler.h"

namespace hiraeth {
	namespace network {
		Socket m_Socket;
		Address m_ClientAddress[MaxClients];
		BufferType m_Buffer[BufferSize];
		size_t m_Size;
		//Socket SocketHandler::m_Socket;
		//Address SocketHandler::m_ClientAddress[MaxClients];
		//BufferType SocketHandler::m_Buffer[BufferSize];
		//size_t SocketHandler::m_Size;
	}
}
