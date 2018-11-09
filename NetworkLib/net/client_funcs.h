#pragma once
#include "protocol.h"

namespace hiraeth {
	namespace network {

		inline int construct_client_packet(BufferType * buffer, unsigned char msgId,
			unsigned int id, const BufferType * msg, size_t message_len)
		{
			const int BUF_LEN = 5 + message_len;
			memset(buffer, '\0', BUF_LEN);
			memcpy(buffer, &msgId, sizeof(char));
			memcpy(buffer + 1, &id, sizeof(unsigned int));
			memcpy(buffer + 5, msg, message_len);
			return  BUF_LEN;
		}
	}
}
