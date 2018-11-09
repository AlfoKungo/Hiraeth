#pragma once
#include "protocol.h"

namespace hiraeth {
	namespace network {


		inline int construct_server_packet(BufferType * buffer, unsigned char msgId, 
			const char * msg, size_t message_len)
		{
			const int BUF_LEN = 1 + message_len;
			memset(buffer, '\0', BUF_LEN);
			memcpy(buffer, &msgId, sizeof(char));
			memcpy(buffer + 1, msg, message_len);
			return  BUF_LEN;
		}
	}
}
