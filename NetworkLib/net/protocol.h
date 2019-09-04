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
			MSG_CTS_CLOSE_CONNECTION = 0x02,
			MSG_CTS_LOCATION_UPDATE = 0x03,
			MSG_CTS_KA = 0x05,
			MSG_CTS_HIT_MOB = 0x06,
			MSG_CTS_KILL_MOB = 0x07,
			MSG_CTS_NPC_CLICK = 0x08,
			MSG_CTS_DIALOG_NEXT = 0x09,
			MSG_CTS_ACCEPT_QUEST = 0x10,
			MSG_CTS_CHAR_GOT_HIT = 0x11,
			MSG_CTS_CHAR_USE_SKILL_E = 0x12,
			MSG_CTS_CHAR_USE_SKILL_A = 0x13,
			MSG_CTS_PICK_ITEM = 0x14,
			MSG_CTS_DROP_ITEM = 0x15,
			MSG_CTS_INCREASE_SKILL = 0x16;
		//enum MSG_CTS {
		//	MSG_CTS_ACK = 0x00,
		//	MSG_CTS_OPEN_CONNECTION,
		//	MSG_CTS_CLOSE_CONNECTION,
		//	MSG_CTS_LOCATION_UPDATE,
		//	MSG_CTS_KA,
		//	MSG_CTS_HIT_MOB,
		//	MSG_CTS_KILL_MOB,
		//	MSG_CTS_NPC_CLICK,
		//	MSG_CTS_DIALOG_NEXT,
		//	MSG_CTS_ACCEPT_QUEST,
		//	MSG_CTS_CHAR_GOT_HIT,
		//	MSG_CTS_CHAR_USE_SKILL_E,
		//	MSG_CTS_CHAR_USE_SKILL_A,
		//};

		PROTOCOL_CODE
			MSG_STC_ACK = 0x40,
			MSG_STC_ESTABLISH_CONNECTION = 0x41,
			MSG_STC_ADD_PLAYER = 0x42,
			MSG_STC_PLAYERS_LOCATIONS = 0x43,
			MSG_STC_PLAYERS_LIST = 0x44,
			MSG_STC_MOB_HIT = 0x45,
			MSG_STC_MOB_DIED = 0x46,
			MSG_STC_NPC_CLICK_ANSWER = 0x47,
			MSG_STC_NPC_QUEST_ACCEPT = 0x48,
			MSG_STC_MOB_DATA = 0x49,
			MSG_STC_MOB_UPDATE = 0x4A,
			MSG_STC_START_DIALOG = 0x4B,
			MSG_STC_DIALOG_NEXT_ANSWER = 0x4C,
			MSG_STC_ACCEPT_QUEST_ACK = 0x4D,
			MSG_STC_CHAR_USE_SKILL_E = 0x4E,
			MSG_STC_CHAR_USE_SKILL_A = 0x4F,
			MSG_STC_PICK_ITEM = 0x50,
			MSG_STC_DROP_ITEM = 0x51,
			MSG_STC_DROPPED_ITEM = 0x52,
			MSG_STC_EXPIRE_ITEM = 0x53,
			MSG_STC_INCREASE_SKILL = 0x54;
		//enum MSG_STC {
		//	MSG_STC_ACK = 0x60,
		//	MSG_STC_ESTABLISH_CONNECTION,
		//	MSG_STC_ADD_PLAYER,
		//	MSG_STC_PLAYERS_LOCATIONS,
		//	MSG_STC_PLAYERS_LIST,
		//	MSG_STC_MOB_HIT,
		//	MSG_STC_MOB_DIED,
		//	MSG_STC_NPC_CLICK_ANSWER,
		//	MSG_STC_NPC_QUEST_ACCEPT,
		//	MSG_STC_MOB_DATA,
		//	MSG_STC_MOB_UPDATE,
		//	MSG_STC_START_DIALOG,
		//	MSG_STC_DIALOG_NEXT_ANSWER,
		//	MSG_STC_ACCEPT_QUEST_ACK,
		//	MSC_STC_CHAR_USE_SKILL_E,
		//	MSC_STC_CHAR_USE_SKILL_A,
		//};

		PROTOCOL_CODE
			MSG_INR_UPDATE_MOB_CMD = 0x71,
			MSG_INR_MOB_HIT = 0x72,
			MSG_INR_MOB_UPDATE = 0x73,
			MSG_INR_ROUTINE_UPDATE = 0x24;
 			//#define MSG_INR_FIND_MOB_POS 0xA2

		PROTOCOL_CODE
			USE_ITEM = 0,
			EQUIP_ITEM = 1;
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
		struct ItemDropMsg
		{
			unsigned int item_id{}, item_type_id{}, item_kind{};
			maths::vec2 location;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(item_id), CEREAL_NVP(item_type_id),
					CEREAL_NVP(item_kind), CEREAL_NVP(location));
			}
		};
		struct MonsterDiedMsg
		{
			unsigned int monster_id;
			//std::vector<unsigned int> dropped_items;
			std::vector<ItemDropMsg> dropped_items;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(monster_id), CEREAL_NVP(dropped_items));
			}
		};

		struct SkillAlloc
		{
			unsigned int skill_id;
			unsigned int pts_alloc;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(skill_id), CEREAL_NVP(pts_alloc));
			}
		};
		struct PlayerStats
		{
			std::string name;
			unsigned int char_lvl{};
			unsigned int job{};
			unsigned int exp{};
			unsigned int max_hp{}, hp{};
			unsigned int max_mp{}, mp{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(name), CEREAL_NVP(char_lvl), CEREAL_NVP(job),
					CEREAL_NVP(exp), CEREAL_NVP(hp), CEREAL_NVP(mp));
			}
		};
		struct PlayerData
		{
			PlayerStats player_stats{};
			//std::string name;
			//unsigned int char_lvl{};
			//unsigned int job{};
			//unsigned int exp{};
			//unsigned int hp{};
			//unsigned int mp{};
			std::vector<unsigned int> stats_alloc;
			//std::map<unsigned int, unsigned int> skills_alloc;
			std::vector<SkillAlloc> skills_alloc;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(player_stats),
				//ar(CEREAL_NVP(name), CEREAL_NVP(char_lvl), CEREAL_NVP(job),
				//	CEREAL_NVP(exp), CEREAL_NVP(hp), CEREAL_NVP(mp),
					CEREAL_NVP(stats_alloc), CEREAL_NVP(skills_alloc));
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
		//struct MonsterDamage
		//{
		//	float damage{};
		//	unsigned int monster_id{};
		//	unsigned int attack_id{};
		//	Direction dir{};
		//	template<class A> void serialize(A& ar) {
		//		ar(CEREAL_NVP(damage), CEREAL_NVP(monster_id), CEREAL_NVP(attack_id), CEREAL_NVP(dir));
		//	}
		//};
		struct MonsterHit
		{
			float damage{};
			unsigned int monster_id{};
			Direction dir{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(damage), CEREAL_NVP(monster_id), CEREAL_NVP(dir));
			}
		};

		struct AttackSkillMsg
		{
			unsigned int skill_id{};
			std::vector<MonsterHit> monsters_hit;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(skill_id), CEREAL_NVP(monsters_hit));
			}
		};

		struct CharAttackSkillMsg
		{
			unsigned int char_id{};
			AttackSkillMsg attack_msg;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(char_id), CEREAL_NVP(attack_msg));
			}
		};

		struct PickItemMsg
		{
			unsigned int char_id{};
			unsigned int item_id{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(char_id), CEREAL_NVP(item_id));
			}
		};


	}
}
