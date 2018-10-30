#pragma once
#include "cereal/archives/binary.hpp"
#include <map>
#include "maths/vec2.h"

namespace hiraeth {
	namespace network {

#define MSG_CTS_OPEN_CONNECTION 0x01
#define MSG_CTS_CLOSE_CONNECTION 0x02
#define MSG_CTS_LOCATION_UPDATE 0x03
#define MSG_CTS_KA 0x05

#define MSG_STC_ADD_PLAYER 0x11
#define MSG_STC_PLAYERS_LOCATIONS 0x12
#define MSG_STC_PLAYERS_LIST 0x13

#define MSG_INR_SUMMON_MONSTER 0x21

		enum MessageType
		{
			open_connection = 0x01,
			close_connection = 0x02,
			location_update = 0x03,
			keep_alive = 0x05,
		};

		enum Direction {
			Right = 1,
			Left = -1,
		};

		struct PlayerStateUpdate
		{
			maths::vec2 pos;
			maths::vec2 force;
			Direction direction{Right};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(pos), CEREAL_NVP(force), CEREAL_NVP(direction));
			}
			//bool left;
			//bool right;
		};

		struct MonsterStateUpdate
		{
			maths::vec2 pos;
			maths::vec2 force;
			bool left;
			bool right;
		};
		struct RegularMapUpdate
		{
			std::map<unsigned int, PlayerStateUpdate> m_PlayersLocation;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(m_PlayersLocation));
			}
		};

		template<class T>
		std::tuple<char *, int> srl_packet_data(const T& srl_data)
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

		/*
		 * a function for deserializing simple data structs
		 */
		template<class T>
		T dsrl_packet_data(char * buffer)
		{
			T dsrl_data;
			memcpy(&dsrl_data, buffer, sizeof(dsrl_data));
			return std::move(dsrl_data);
		}

		template<class T>
		void dsrl_nr_packet_data(T& dsrl_data, char * buffer)
		{
			memcpy(&dsrl_data, buffer, sizeof(dsrl_data));
		}

		/*
		 * void dsrl_d(ynamic)t(ype)_packet_data(T& dsrl_data, char * buffer)
		 * 
		 * a function for deserializing dynamic data types. requires the buffer
		 * to contain the size of the data in the first slot.
		 */
		template<class T>
		void dsrl_dt_packet_data(T& dsrl_data, char * buffer)
		{
			const std::string tmp_str{ buffer + 1, static_cast<unsigned int>(buffer[0]) };
			std::stringstream is(tmp_str, std::ios::binary | std::ios::in);
			{
				cereal::BinaryInputArchive ar(is);
				ar(dsrl_data);
			}
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
