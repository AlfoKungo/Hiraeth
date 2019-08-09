#pragma once
#include "cereal/archives/binary.hpp"
#include <map>
#include "maths/vec2.h"

namespace hiraeth {
	namespace network {
#define PROTOCOL_CODE const unsigned char
		PROTOCOL_CODE
			MSG_CTS_ACK = 0x00,
			MSG_CTS_OPEN_CONNECTION = 0x01,
			MSG_CTS_CLOSE_CONNECTION = 0x0,
			MSG_CTS_LOCATION_UPDATE = 0x03,
			MSG_CTS_KA = 0x05,
			MSG_CTS_HIT_MOB = 0x06,
			MSG_CTS_KILL_MOB = 0x07,
			MSG_CTS_NPC_CLICK = 0x08,
			MSG_CTS_DIALOG_NEXT = 0x09,
			MSG_CTS_ACCEPT_QUEST = 0x10;

		PROTOCOL_CODE
			MSG_STC_ACK = 0x60,
			MSG_STC_ESTABLISH_CONNECTION = 0x61,
			MSG_STC_ADD_PLAYER = 0x62,
			MSG_STC_PLAYERS_LOCATIONS = 0x63,
			MSG_STC_PLAYERS_LIST = 0x64,
			MSG_STC_MOB_HIT = 0x65,
			MSG_STC_MOB_DIED = 0x66,
			MSG_STC_NPC_CLICK_ANSWER = 0x67,
			MSG_STC_NPC_QUEST_ACCEPT = 0x68,
			MSG_STC_MOB_DATA = 0x71,
			MSG_STC_MOB_UPDATE = 0x72,
			MSG_STC_START_DIALOG = 0x73,
			MSG_STC_DIALOG_NEXT_ANSWER = 0x74,
			MSG_STC_ACCEPT_QUEST_ACK = 0x75;

		PROTOCOL_CODE
			MSG_INR_UPDATE_MOB_CMD = 0xA1,
			MSG_INR_MOB_HIT = 0xA2,
			MSG_INR_MOB_UPDATE = 0xA3;
 			//#define MSG_INR_FIND_MOB_POS 0xA2

		using BufferType = char;

		enum Direction {
			Right = 1,
			Stand = 0,
			Left = -1,
		};

		struct PlayerStateUpdateMsg
		{
			maths::vec2 pos;
			maths::vec2 force;
			Direction direction{ Right };
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
			unsigned int hp{};
			//bool left{};
			//bool right{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(monster_type), CEREAL_NVP(pos),
					CEREAL_NVP(force), CEREAL_NVP(dir), CEREAL_NVP(hp));
			}
		};
		struct MonsterStateUpdateMsg // MSG_STC_MOB_UPDATE
		{
			unsigned int monster_id{};
			MonsterStateUpdate monster_state;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(monster_id), CEREAL_NVP(monster_state));
			}
		};
		struct RegularMapUpdate
		{
			std::map<unsigned int, PlayerStateUpdateMsg> m_PlayersLocation;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(m_PlayersLocation));
			}
		};
		struct MonsterDiedMsg
		{
			unsigned int monster_id;
			std::vector<unsigned int> dropped_items;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(monster_id), CEREAL_NVP(dropped_items));
			}
		};
		struct PlayerData
		{
			unsigned int char_lvl{};
			std::string name;
			unsigned int job{};
			std::vector<unsigned int> stat_allocation;
			std::vector<unsigned int> skill_allocation;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(char_lvl), CEREAL_NVP(name), CEREAL_NVP(job),
					CEREAL_NVP(stat_allocation), CEREAL_NVP(skill_allocation));
			}
		};
		struct ConnectionEstablishMsg
		{
			unsigned int client_id{};
			PlayerData player_data{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(client_id), CEREAL_NVP(player_data));
			}
		};
		struct MonsterDamage
		{
			float damage{};
			unsigned int monster_id{};
			unsigned int attack_id{};
			Direction dir{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(damage), CEREAL_NVP(monster_id), CEREAL_NVP(attack_id), CEREAL_NVP(dir));
			}
		};


	}
}
