#pragma once
#include "cereal/archives/binary.hpp"

namespace hiraeth {
	namespace network {

#define MSG_CTS_OPEN_CONNECTION 0x01
#define MSG_CTS_CLOSE_CONNECTION 0x02
#define MSG_CTS_LOCATION_UPDATE 0x03
#define MSG_CTS_KA 0x05

#define MSG_STC_PLAYERS_LOCATIONS 0x11
		enum MessageType
		{
			open_connection = 0x01,
			close_connection = 0x02,
			location_update = 0x03,
			keep_alive = 0x05,
		};

		template<class T>
		std::tuple<char *, int> serialize_packet_data(T& srl_data)
		{
			std::stringstream os{ std::ios::binary | std::ios::out };
			{
				cereal::BinaryOutputArchive ar(os);
				ar(srl_data);
			} // the binary archives will flush their output 
			std::string data_str = os.str();
			auto data = new char[data_str.size() + 1];
			data[0] = char(data_str.size());
			memcpy(data + 1, data_str.c_str(), data_str.size());
			return { data, data_str.size() + 1};
		}

		inline int construct_client_packet(char * buffer, unsigned char msgId,
			unsigned int id, const char * msg, size_t message_len)
		{
			const int BUF_LEN = 5 + message_len;
			memset(buffer, '\0', BUF_LEN);
			memcpy(buffer, &msgId, sizeof(char));
			memcpy(buffer + 1, &id, sizeof(unsigned int));
			memcpy(buffer + 5, msg, message_len);
			return  BUF_LEN;
		}

		inline int construct_server_packet(char * buffer, unsigned char msgId, 
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