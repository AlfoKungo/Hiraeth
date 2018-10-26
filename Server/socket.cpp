#include "socket.h"

namespace hiraeth {
	namespace network {

		Socket::Socket()
		{
			WSADATA wsa;
			printf("\nInitialising Winsock...");
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			{
				printf("Failed. Error Code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			if ((handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
			{
				printf("Could not create socket : %d", WSAGetLastError());
			}
		}

		Socket::~Socket()
		{
			Close();
			WSACleanup();
		}

		bool Socket::Open(unsigned short port)
		{
			struct sockaddr_in server {};
			server.sin_family = AF_INET;
			server.sin_addr.s_addr = INADDR_ANY;
			server.sin_port = htons(port);

			//Bind
			if (bind(handle, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) == SOCKET_ERROR)
			{
				printf("Bind failed with error code : %d", WSAGetLastError());
				return false;
				//exit(EXIT_FAILURE);
			}
			printf("Socket created.\n");
			return true;
		}

		void Socket::SetUnblocking() const
		{
			DWORD nonBlocking = 1;
			if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
			{
				printf("failed to set non-blocking\n");
			}
		}

		void Socket::Close()
		{
			closesocket(handle);
		}

		bool Socket::IsOpen() const
		{
			return handle != 0;
		}

		bool Socket::Send(const Address& destination, const void* data, int size)
		{
			struct sockaddr_in client { destination.GetStruct() };
			const int client_len = sizeof(client);

			const auto ucharptr = reinterpret_cast<char*>(const_cast<void*>(data));

			if (sendto(handle, ucharptr, size, 0, reinterpret_cast<struct sockaddr*>(&client), client_len) == SOCKET_ERROR)
			{
				printf("sendto() failed with error code : %d", WSAGetLastError());
				return false;
				//exit(EXIT_FAILURE);
			}
			return true;
		}

		int Socket::Receive(Address& sender, void* data, int size)
		{
			struct sockaddr_in client {};

			int client_len = sizeof(client), recv_len;

			const auto ucharptr = reinterpret_cast<char*>(const_cast<void*>(data));

			//DWORD Total;
			//FD_SET WriteSet;
			//FD_SET ReadSet;
			//if ((Total = select(0, &ReadSet, &WriteSet, NULL, NULL)) == SOCKET_ERROR)
			//{

			//	printf("select() returned with error %d\n", WSAGetLastError());

			//	return 1;

			//}
			//if (FD_ISSET(ListenSocket, &ReadSet))

   //   {
   //      Total--;
   //      if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
   //      {
   //         // Set the accepted socket to non-blocking mode so the server will
   //         // not get caught in a blocked condition on WSASends
   //         NonBlock = 1;
   //         if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
   //         {
   //            printf("ioctlsocket(FIONBIO) failed with error %d\n", WSAGetLastError());
   //            return 1;
   //         }
   //         else
   //            printf("ioctlsocket(FIONBIO) is OK!\n");
 
   //         if (CreateSocketInformation(AcceptSocket) == FALSE)
   //         {
   //              printf("CreateSocketInformation(AcceptSocket) failed!\n");
   //              return 1;
   //         }
   //         else
   //             printf("CreateSocketInformation() is OK!\n");
 
   //      }
   //      else
   //      {
   //         if (WSAGetLastError() != WSAEWOULDBLOCK)
   //         {
   //            printf("accept() failed with error %d\n", WSAGetLastError());
   //            return 1;
   //         }
   //         else
   //            printf("accept() is fine!\n");
   //      }


			if ((recv_len = recvfrom(handle, ucharptr, size, 0, reinterpret_cast<struct sockaddr *>(&client), &client_len)) == SOCKET_ERROR)
			{
				printf("recvfrom() failed with error code : %d", WSAGetLastError());
				return 0;
				//exit(EXIT_FAILURE);
			}
			sender = Address{ client };
			return recv_len;
		}
	}
}