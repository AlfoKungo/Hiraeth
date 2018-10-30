/*
    Simple udp client
*/
#include<cstdio>
#include<winsock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>

#include <chrono>
#include <thread>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
 
int main(void)
{
	struct sockaddr_in si_other {};
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
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

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		strcpy_s(message, "hello");
		if (sendto(s, message, strlen(message), 0, reinterpret_cast<struct sockaddr *>(&si_other), slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			return -1;
		}
		//printf("sent keep_alive message\n");
	    memset(buf,'\0', BUFLEN);
	    if (recvfrom(s, buf, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&si_other), &slen) == SOCKET_ERROR)
	    {
	        printf("recvfrom() failed with error code : %d" , WSAGetLastError());
			return -1;
	    }
		printf("received ack message\n");
	}

	closesocket(s);
	WSACleanup();

	return 0;
}