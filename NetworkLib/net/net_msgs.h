#pragma once
#include "protocol.h"
#include <srl\npc_data.h>

namespace hiraeth::network {

	struct MsgStcNewPlayerInMap //dynamic
	{
		unsigned int player_id{};
		//decltype(PlayerData::equips_char) player_equips;
		std::map<SRL::EquipItemType, unsigned int> player_equips;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(player_id), CEREAL_NVP(player_equips));
		}
	};
	struct MsgStcUpdateNetCharEquips //dynamic
	{
		unsigned int player_id{};
		//decltype(PlayerData::equips_char) player_equips;
		std::map<SRL::EquipItemType, unsigned int> player_equips;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(player_id), CEREAL_NVP(player_equips));
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
	struct MsgStcNpcStartDialog
	{
		unsigned int npc_id{}, dialog_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_id), CEREAL_NVP(dialog_id));
		}
	};
	struct MsgCtsDialogButtonClick
	{
		unsigned int npc_id{}, dialog_id{}, sub_dialog_id{}, button_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_id), CEREAL_NVP(dialog_id), CEREAL_NVP(sub_dialog_id), CEREAL_NVP(button_id));
		}
	};
	struct MsgStcDialogNextSubDialog
	{
		//unsigned int npc_id{}, dialog_id{}, sub_dialog_id{};
		template<class A> void serialize(A& ar) {
			//ar(CEREAL_NVP(npc_id), CEREAL_NVP(dialog_id), CEREAL_NVP(sub_dialog_id));
		}
	};
	struct MsgStcDialogSet
	{
		unsigned int npc_id{}, dialog_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_id), CEREAL_NVP(dialog_id));
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

	struct AddItemMsg
	{
		unsigned int item_kind{}, item_loc{}, item_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_kind), CEREAL_NVP(item_loc), CEREAL_NVP(item_id));
		}
	};
	struct AddEquipItemMsg
	{
		unsigned int item_loc{}, item_id{};
		SRL::EquipDbStruct item_info;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_id), CEREAL_NVP(item_loc), CEREAL_NVP(item_info));
		}
	};
	struct MsgCtsDropItem
	{
		unsigned int item_kind{}, item_type_id{}, item_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_kind), CEREAL_NVP(item_type_id),
				CEREAL_NVP(item_id));
		}
	};
	struct MsgStcDropItem
	{
		unsigned int item_kind{}, item_type_id{}, item_id{};
		maths::vec2 pos{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_kind), CEREAL_NVP(item_type_id),
				CEREAL_NVP(item_id), CEREAL_NVP(pos));
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
	struct EnterPortalMsg
	{
		unsigned int next_map{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(next_map));
		}
	};
	struct ChangeMapMsg
	{
		unsigned int next_map{};
		unsigned int new_foothold{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(next_map), CEREAL_NVP(new_foothold));
		}
	};
	struct PlayerLeftMsg
	{
		unsigned int player_left_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(player_left_id));
		}
	};
	struct MsgCtsTradeRequest
	{
		//unsigned char msg_id{ MSG_CTS_TRADE_REQUEST };
		IdType player_to_trade{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(player_to_trade));
		}
	};
	struct MsgStcTradeRequest
	{
		IdType player_who_wants_to_trade{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(player_who_wants_to_trade));
		}
	};
	struct MsgCtsTradeRequestAccept
	{
		IdType player_who_wants_to_trade{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(player_who_wants_to_trade));
		}
	};
	struct MsgStcTradeFacilitate
	{
		IdType player_to_trade_with{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(player_to_trade_with));
		}
	};
	struct MsgCtsTradeAddItemToBox
	{
		unsigned int tab_index{};
		unsigned int inventory_index{};
		unsigned int trade_index{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(tab_index), CEREAL_NVP(inventory_index), CEREAL_NVP(trade_index));
		}
	};
	struct MsgStcTradeAddItemToBox
	{
		unsigned int trade_index{};
		SRL::EquipDbStruct equip_info{};
		bool is_me{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(trade_index), CEREAL_NVP(equip_info), CEREAL_NVP(is_me));
		}
	};
	struct MsgStcTradePlayerAccepted
	{
		bool is_me{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(is_me));
		}
	};

	struct MsgStcShopOpen
	{
		std::vector<SRL::MerchantItemSellData> sell_items;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(sell_items));
		}
	};
	struct MsgCtsShopBuyItem
	{
		unsigned int item_num;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_num));
		}
	};
}
