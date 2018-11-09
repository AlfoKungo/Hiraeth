 
#include "server.h"
 
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main()
{
	  // create socket
	using namespace hiraeth::network;


	Server server;
	//server.main_60fps_loop();
	//server.main_block_receive_and_async_send();
	//server.main_block_threaded_queue();
	server.main_60fps_threaded_queue();


	return 0;
}
