#pragma once
#include "cereal/archives/binary.hpp"
#include <map>
#include "maths/vec2.h"
#include "srl/item_data.h"
#include "srl/equip_item_data.h"
#include "srl/quest_data.h"

namespace hiraeth {
	namespace network {
#define PROTOCOL_CODE const char
		PROTOCOL_CODE
			MSG_CTS_ACK = 0x00,
			MSG_CTS_OPEN_CONNECTION = -0x01,
			MSG_CTS_CLOSE_CONNECTION = -0x02,
			MSG_CTS_LOCATION_UPDATE = -0x03,
			MSG_CTS_KA = -0x05,
			MSG_CTS_HIT_MOB = -0x06,
			MSG_CTS_KILL_MOB = -0x07,
			MSG_CTS_NPC_CLICK = -0x08,
			MSG_CTS_DIALOG_NEXT = -0x09,
			MSG_CTS_ACCEPT_QUEST = -0x10,
			MSG_CTS_CHAR_GOT_HIT = -0x11,
			MSG_CTS_CHAR_USE_SKILL_E = -0x12,
			MSG_CTS_CHAR_USE_SKILL_A = -0x13,
			MSG_CTS_PICK_ITEM = -0x14,
			MSG_CTS_DROP_ITEM = -0x15,
			MSG_CTS_INCREASE_SKILL = -0x16,
			MSG_CTS_WEAR_EQUIP = -0x17,
			MSG_CTS_USE_ITEM = -0x18,
			MSG_CTS_INVENTORY_ACTION = -0x19,
			MSG_CTS_PLAYER_SAY = -0x20,
			MSG_CTS_FINISH_QUEST = -0x21,
			MSG_CTS_PARTY_REQUEST = -0x22,
			MSG_CTS_ACCEPT_PARTY = -0x23,
			MSG_CTS_DECLINE_PARTY = -0x24;

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
			MSG_STC_DROPPED_ITEMS = 0x52,
			MSG_STC_EXPIRE_ITEM = 0x53,
			MSG_STC_ADD_ITEM_TO_INVENTORY = 0x54,
			MSG_STC_INCREASE_SKILL = 0x55,
			MSG_STC_PLAYER_SAY = 0x56,
			MSG_STC_SET_QUEST_IP = 0x57, // IP = In progress
			MSG_STC_FINISH_QUEST = 0x58,
			MSG_STC_PARTY_REQUEST = 0x59,
			MSG_STC_UPDATE_PARTY_STATE = 0x60,
			MSG_STC_RECEIVED_EXP = 0x61;

		PROTOCOL_CODE
			MSG_INR_UPDATE_MOB_CMD = 0x71,
			MSG_INR_MOB_HIT = 0x72,
			MSG_INR_MOB_UPDATE = 0x73,
			MSG_INR_ROUTINE_UPDATE = 0x24;
 			//#define MSG_INR_FIND_MOB_POS 0xA2

		PROTOCOL_CODE
			EQUIP_ITEM = 0,
			USE_ITEM = 1,
			SETUP_ITEM = 2,
			ETC_ITEM = 3,
			CASH_ITEM = 4;
		using BufferType = char;
		const int BufferSize = 512;

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

		struct KillStruct
		{
			//unsigned int mob_type;
			unsigned int kill_amount{ 0 };
			unsigned int target_amount{};
			KillStruct() = default;
			KillStruct(SRL::QuestDouble qd)
			{
				target_amount = qd.amount;
			}
			bool checkIfAccomplished() const
			{
				return kill_amount == target_amount;
			}
			template<class A> void serialize(A& ar) {
				//ar(CEREAL_NVP(mob_type), CEREAL_NVP(kill_amount), 
				ar(CEREAL_NVP(kill_amount),
					CEREAL_NVP(target_amount));
			}
		};
		
		struct PlayerHoldState
		{
			using MobId = unsigned int;
			std::map<unsigned int, std::map<MobId, KillStruct>> active_kill_quests;
			std::map<unsigned int, unsigned int> npc_dialog_id;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(active_kill_quests), CEREAL_NVP(npc_dialog_id));
			}
		};

		struct PlayerMsg
		{
			//unsigned int msg_id;
			//BufferType msg_buffer[BufferSize];
			BufferType* msg_buffer;
			int buffer_size{};
			~PlayerMsg()
			{
				delete[] msg_buffer;
				//free(msg_buffer);
			}
		};

		struct PlayerMsgState 
		{
			unsigned int crnt_id;
			//std::vector<PlayerMsg> player_msgs;
			std::map<unsigned int, PlayerMsg> player_msgs;
			unsigned int get_ack_id()
			{
				return crnt_id++;
			}
			void add_new_msg(BufferType * buffer, int buffer_size, unsigned int ack_id)
			{
				player_msgs.insert({ ack_id, PlayerMsg{{}, buffer_size } });
				//player_msgs[ack_id].msg_buffer = (BufferType*)malloc(buffer_size);
				player_msgs[ack_id].msg_buffer = new BufferType[BufferSize];
				memcpy(player_msgs[ack_id].msg_buffer, buffer, buffer_size);
				//player_msgs[ack_id].msg_buffer
			}
		};

		
		struct PlayerData
		{
			PlayerStats player_stats{};
			std::vector<unsigned int> stats_alloc;
			std::vector<SkillAlloc> skills_alloc;
			std::map<unsigned int, unsigned int> inv_equip;
			std::map<unsigned int, SRL::ItemDbStruct> inv_use;
			std::map<unsigned int, SRL::ItemDbStruct> inv_setup;
			std::map<unsigned int, SRL::ItemDbStruct> inv_etc;
			std::map<unsigned int, SRL::ItemDbStruct> inv_cash;
			std::map<SRL::EquipItemType, unsigned int> equips_char;
			PlayerHoldState player_hold_state;
			std::vector<SRL::QuestDBStruct> quests_in_progress;
			std::vector<unsigned int> quests_done;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(player_stats), CEREAL_NVP(stats_alloc),
					CEREAL_NVP(skills_alloc), CEREAL_NVP(inv_equip),
					CEREAL_NVP(inv_use), CEREAL_NVP(inv_setup),
					CEREAL_NVP(inv_etc), CEREAL_NVP(inv_cash),
					CEREAL_NVP(equips_char), CEREAL_NVP(player_hold_state),
					CEREAL_NVP(quests_in_progress), CEREAL_NVP(quests_done));
					//CEREAL_NVP(skills_alloc), CEREAL_NVP(items),
					//CEREAL_NVP(equips_inv), CEREAL_NVP(equips_char));
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
		struct msgStcSetQuestIp
		{
			unsigned int ack_id{}, quest_id{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(ack_id), CEREAL_NVP(quest_id));
			}
		};
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
		struct PlayerSayMsg
		{
			unsigned int char_id{};
			std::string say_msg{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(char_id), CEREAL_NVP(say_msg));
			}
		};
		struct FinishQuestMsg
		{
			unsigned int quest_id{};
			unsigned int exp{};
			unsigned int money{};
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(exp), CEREAL_NVP(money));
			}
		};
		struct PartyUpdateMsg
		{
			std::vector<std::tuple<unsigned int, std::string>> party_members;
			unsigned int ack;
			template<class A> void serialize(A& ar) {
				ar(CEREAL_NVP(party_members), CEREAL_NVP(ack));
			}
		};

	}
}
