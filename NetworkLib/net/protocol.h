#pragma once
#include "cereal/archives/binary.hpp"
#include <map>
#include "maths/vec2.h"

namespace hiraeth {
	namespace network {

#define MSG_CTS_ACK 0x00
#define MSG_CTS_OPEN_CONNECTION 0x01
#define MSG_CTS_CLOSE_CONNECTION 0x02
#define MSG_CTS_LOCATION_UPDATE 0x03
#define MSG_CTS_KA 0x05

#define MSG_STC_ACK 0x10
#define MSG_STC_ADD_PLAYER 0x11
#define MSG_STC_PLAYERS_LOCATIONS 0x12
#define MSG_STC_PLAYERS_LIST 0x13

#define MSG_STC_MOB_DATA 0x21
#define MSG_STC_MOB_UPDATE 0x22

#define MSG_INR_UPDATE_MOB_CMD 0xA1
#define MSG_INR_FIND_MOB_POS 0xA2
#define MSG_INR_MOB_UPDATE 0xA3

		using BufferType = char;

		enum MsgCTS
		{
			open_connection = 0x01,
			close_connection = 0x02,
			location_update = 0x03,
			keep_alive = 0x05,
		};

		enum Direction {
			Right = 1,
			Stand = 0,
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

		struct MonsterStateUpdate // MSG_STC_MOB_DATA
		{
			unsigned int monster_type{};
			maths::vec2 pos;
			maths::vec2 force;
			Direction dir{};
			//bool left{};
			//bool right{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(monster_type), CEREAL_NVP(pos),
					CEREAL_NVP(force), CEREAL_NVP(dir));
			}
		};
		struct MonsterStateDataUpdate // MSG_STC_MOB_UPDATE
		{
			unsigned int monster_id{};
			MonsterStateUpdate monster_state;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(monster_id), CEREAL_NVP(monster_state));
			}
		};
		struct RegularMapUpdate
		{
			std::map<unsigned int, PlayerStateUpdate> m_PlayersLocation;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(m_PlayersLocation));
			}
		};


	}
}
