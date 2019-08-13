#pragma once
#include "protocol.h"

namespace hiraeth {
	namespace network {

		inline int create_client_packet_with_buffer(BufferType * buffer, unsigned char msgId,
			const BufferType * msg, size_t message_len)
		{
			const int BUF_LEN = 1 + message_len;
			memset(buffer, '\0', BUF_LEN);
			memcpy(buffer, &msgId, sizeof(char));
			memcpy(buffer + 1, msg, message_len);
			return  BUF_LEN;
		}
		inline int create_client_packet_with_buffer(BufferType * buffer, unsigned char msgId,
			unsigned int id, const BufferType * msg, size_t message_len)
		{
			const int BUF_LEN = 5 + message_len;
			memset(buffer, '\0', BUF_LEN);
			memcpy(buffer, &msgId, sizeof(char));
			memcpy(buffer + 1, &id, sizeof(unsigned int));
			memcpy(buffer + 5, msg, message_len);
			return  BUF_LEN;
		}

		//template<typename ... Ts>
		//inline int create_client_packet_with_data(BufferType * buffer, unsigned char msgId,
		//	unsigned int id, Ts&&... dts)
		//{
		//	//auto message_len = srl_types(buffer + 5, dts...);
		//	//memset(buffer, '\0', 5 + message_len);
		//	memcpy(buffer, &msgId, sizeof(char));
		//	memcpy(buffer + 1, &id, sizeof(unsigned int));
		//	//message_len = srl_types(buffer + 5, dts...);
		//	auto message_len = srl_types(buffer + 5, dts...);
		//	return 5 + message_len;
		//}
		template<typename ... Ts>
		inline int create_client_packet_with_data(BufferType * buffer,
			Ts&&... dts)
		{
			return srl_types(buffer, dts...);
		}
	}
}
