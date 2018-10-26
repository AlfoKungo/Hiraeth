 
#include "server.h"
 
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main()
{
	  // create socket
	using namespace hiraeth::network;


	Server server;
	server.main_function();

	return 0;
}
