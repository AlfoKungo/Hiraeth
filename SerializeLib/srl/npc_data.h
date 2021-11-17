#pragma once
#include <string>
#include "texture_data.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include "maths/maths.h"
//#include <variant>
#include <cereal/types/variant.hpp>

namespace SRL {
	//#define DIALOG_TYPE const char
	enum class DialogButton {
		Next,
		Accept,
		Cancel,
		Ok,
		ReceiveReward,
		Text,
	};
	//using DIALOG_BUTTON_TYPE=char;
	//const DIALOG_BUTTON_TYPE
	//	RT_NEXT = 0,
	//	RT_ACCEPT = 1,
	//	RT_CANCEL = 2,
	//	RT_OK = 3,
	//	RT_RECEIVE_REWARD = 4;
	const unsigned int
		NPC_DIALOG_DEFAULT = 0,
		NPC_DIALOG_QUEST_OFFER = 1,
		NPC_DIALOG_QUEST_IN_PROG = 2,
		NPC_DIALOG_QUEST_FINISH = 3;
	const unsigned int
		//NPC_DIALOG_DEFAULT = 0,
		NPC_DIALOG_PQ_OFFER = 1,
		NPC_DIALOG_PQ_DENIED = 2,
		NPC_DIALOG_PQ_FINISH = 3;

	enum class DialogType
	{
		quest
	};
	struct DANextDialog // DA = Dialog Action
	{
		template<class A> void serialize(A& ar) {
		}
	};
	struct DAAddQuest
	{
		unsigned int quest_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(quest_id));
		}
	};
	struct DAReceiveReward
	{
		unsigned int quest_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(quest_id));
		}
	};
	struct DAGoToDialog
	{
		unsigned int next_map{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(next_map));
		}
	};
	struct DAChangeMap
	{
		unsigned int next_map{};
		bool is_to_include_party{false};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(next_map), CEREAL_NVP(is_to_include_party));
		}
	};
	struct DASetJob
	{
		unsigned int job_id{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(job_id));
		}
	};
	using ActionVariant = std::variant<DANextDialog, DAAddQuest, DAReceiveReward, DAGoToDialog, DAChangeMap, DASetJob>;

	struct DialogButtonType
	{
		DialogButton button;
		ActionVariant action{ DANextDialog{} };
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(button), CEREAL_NVP(action));
		}
	};
	struct DialogTextButtons
	{
		std::string text;
		//std::variant<DANextDialog> action;
		ActionVariant action{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(text), CEREAL_NVP(action));
		}
	};
	struct DialogStruct
	{
		std::string text;
		//std::vector<DialogButton> buttons;
		std::vector<DialogButtonType> buttons{ {DialogButton::Next} };
		std::vector<DialogTextButtons> text_buttons{};
		//std::map<DialogType, std::variant<int>> props;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(text), CEREAL_NVP(buttons), CEREAL_NVP(text_buttons));
		}
	};
	//enum NpcProp
	//{
	//	np_Quest, // struct that contains {lvl_required, prior_quests_required, quest_id}
	//	np_PQ, // struct that containes {amount_required, lvls_required, pq_map_id}
	//	np_JobAdvancement, // struct that contains {lvl_required}
	//	np_Merchant, //struct that contains {vector<item_for_sell_data>}

	//};
	struct DR_Lvl // DR = Dialog Requirement
	{
		unsigned int min_lvl;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(min_lvl));
		}
	};
	struct DR_ActiveQuest
	{
		unsigned int active_quest_id;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(active_quest_id));
		}
	};
	struct DR_QuestFulfilled
	{
		unsigned int quest_id;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(quest_id));
		}
	};
	struct DR_PriorQuest
	{
		unsigned int prior_quest_id;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(prior_quest_id));
		}
	};
	struct DR_JobRequirements
	{
		unsigned int req_job_id;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(req_job_id));
		}
	};
	struct DR_DontCont
	{
		template<class A> void serialize(A& ar) {
		}
	};
	struct DR_Pq
	{
		unsigned int min_players{}, max_players{};
		unsigned int min_lvl{}, max_lvl{};

		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(min_players), CEREAL_NVP(max_players),
			CEREAL_NVP(min_lvl), CEREAL_NVP(max_lvl));
		}
	};

	using ReqVariant = std::variant<
		DR_Lvl,
		DR_ActiveQuest,
		DR_QuestFulfilled,
		DR_PriorQuest,
		DR_JobRequirements,
		DR_DontCont,
		DR_Pq
	>;

	struct DialogNode
	{
		std::vector<DialogStruct> msgs;
		std::vector<ReqVariant> reqs;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(msgs), CEREAL_NVP(reqs));
		}
	};

	struct NpcUsageDialog
	{
		//std::map<unsigned int, DialogNode> dialog_tree;
		template<class A> void serialize(A& ar) {
			//ar(CEREAL_NVP(dialog_tree));
		}
	};
	struct NpcUsagePq
	{	

		//std::map<unsigned int, DialogNode> dialog_tree;
		unsigned int party_members_amount{}, lvl_req_min{}, lvl_req_max{}, pq_map_id{};
		template<class A> void serialize(A& ar) {
			ar(
				//CEREAL_NVP(dialog_tree), 
				CEREAL_NVP(party_members_amount), CEREAL_NVP(lvl_req_min),
				CEREAL_NVP(lvl_req_max), CEREAL_NVP(pq_map_id));
		}
	};
	//struct NpcJobAdvancementDataStruct
	//{
	//	unsigned int lvl_req{}, job_assignment{};
	//	template<class A> void serialize(A& ar) {
	//		ar(CEREAL_NVP(lvl_req), CEREAL_NVP(job_assignment));
	//	}
	//};
	struct MerchantItemSellData
	{
		unsigned int item_type{}, item_id{}, price{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_type), CEREAL_NVP(item_id), CEREAL_NVP(price));
		}
	};
	struct NpcUsageMerchant
	{
		std::vector<MerchantItemSellData> sell_items;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(sell_items));
		}
	};

	using NpcTypeVariant = std::variant<
		NpcUsageDialog, 
		NpcUsagePq,
		NpcUsageMerchant
	>;

	struct NpcInfo
	{
		std::string npc_name;
		unsigned int npc_foothold;
		float npc_x_value;
		std::vector<std::string> say_lines;
		std::map<unsigned int, DialogNode> dialog_tree;
		NpcTypeVariant usage_data;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_name), CEREAL_NVP(npc_foothold), CEREAL_NVP(npc_x_value),
				CEREAL_NVP(say_lines), CEREAL_NVP(dialog_tree), CEREAL_NVP(usage_data));
		}
	};

	struct NpcData
	{
		NpcInfo info;
		TextureData texture_data;
		unsigned int npc_frames_amount = 1;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(info), CEREAL_NVP(texture_data), CEREAL_NVP(npc_frames_amount));
		}
	};

	struct DialogTreeE
	{
		std::vector<std::variant<std::string, unsigned int>> m_Strings;
	};
}
