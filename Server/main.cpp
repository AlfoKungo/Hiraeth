// Server side C/C++ program to demonstrate Socket programming
//#include <stdio.h>
//#include <socketapi.h>
//#include <stdlib.h>
//#include <string.h>
//#include <wincon.h>
//#include <iostream>
//#include <WS2tcpip.h>
//#include "boost/asio.hpp"
#include<cstdio>
#include<winsock2.h>
 
#define _WIN32_WINNT _WIN32_WINNT_WIN8 // Windows 8.0
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main()
{
	SOCKET s;
    struct sockaddr_in server{}, client{};
    int client_len , recv_len;
    char buf[BUFLEN];
    WSADATA wsa;
 
    client_len = sizeof(client) ;
     
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
     
    //Bind
    if( bind(s ,reinterpret_cast<struct sockaddr *>(&server) , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");
 
    //keep listening for data
        printf("Waiting for data...");
        fflush(stdout);
         
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&client), &client_len)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
         
        //print details of the client/peer and the data received
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
		if (strcmp(buf, "hello") == 0)
		{
			printf("got new client \n");
		}
         
    while(true)
    {
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&client), &client_len)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
		printf("received keep_alive message\n");
        ////now reply the client with the same data
        //if (sendto(s, buf, recv_len, 0, reinterpret_cast<struct sockaddr*>(&client), client_len) == SOCKET_ERROR)
        //{
        //    printf("sendto() failed with error code : %d" , WSAGetLastError());
        //    exit(EXIT_FAILURE);
        //}
    }
 
    closesocket(s);
    WSACleanup();
     
    return 0;
}
