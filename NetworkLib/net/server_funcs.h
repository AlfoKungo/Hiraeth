#pragma once
#include "protocol.h"

namespace hiraeth {
	namespace network {


		inline int construct_server_packet_with_buffer(BufferType * buffer, unsigned char msgId, 
			const char * msg, size_t message_len)
		{
			const int BUF_LEN = 1 + message_len;
			memset(buffer, '\0', BUF_LEN);
			memcpy(buffer, &msgId, sizeof(char));
			memcpy(buffer + 1, msg, message_len);
			return  BUF_LEN;
		}

		template<typename ... Ts>
		inline int construct_server_packet(BufferType * buffer, unsigned char msgId, Ts&&... dts)
		{
			memcpy(buffer, &msgId, sizeof(char));
			auto message_len = srl_types(buffer + 1, dts...);
			return 1 + message_len;
		}

		//template<typename ... Ts>
		//inline int construct_server_packet(BufferType * buffer, MsgEnum cmnd, Ts&&... dts)
		//{
		//	return construct_server_packet(buffer, (unsigned char)cmnd, dts...);
		//}
	}
}
