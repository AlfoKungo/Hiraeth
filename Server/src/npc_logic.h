#pragma once
#include "net/protocol.h"

namespace hiraeth::network {

	// helper type for the visitor #4
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	// explicit deduction guide (not needed as of C++20)
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

	inline bool are_reqs_fulfilled(const decltype(SRL::DialogNode::reqs)& reqs, const PlayerStats& player_stats,
		const decltype(PlayerHoldState::active_kill_quests)& active_quests, 
		const decltype(PlayerHoldState::fulfilled_quests)& fulfilled_quests)
	{
		for (const auto& req : reqs)
		{
			if (!std::visit(overloaded{
				[&](SRL::DR_Lvl data)
			{
				return player_stats.char_lvl >= data.min_lvl;
			},
				[&](SRL::DR_ActiveQuest data)
			{
				return active_quests.find(data.active_quest_id) != active_quests.end();
			},
				[&](SRL::DR_QuestFulfilled data)
			{
				//return data.quest_id <= player_stats.char_lvl;
				//return 
				return std::find(fulfilled_quests.begin(), fulfilled_quests.end(), data.quest_id) != fulfilled_quests.end();
				//return false;
			},
				[&](SRL::DR_PriorQuest data)
			{
				return false;
			},
				[&](SRL::DR_DontCont data)
			{
				return false;
			},
				[&](SRL::DR_Pq data)
			{
				return false;
			}
				}, req))
				return false;

		}
		return true;
	}
	//template <typename T, typename... Args>
	//struct is_one_of :
	//	std::disjunction<std::is_same<std::decay_t<T>, Args>...> {};

	//template <typename... Args>
	//struct visit_only_for {
	//	// delete templated call operator
	//	template <typename T>
	//	std::enable_if_t < !is_one_of<T, Args...>() > operator()(T&&) const = delete;
	//};
			//std::visit([&](auto&& arg) {
			//	using T = std::decay_t<decltype(arg)>;
			//	if constexpr (std::is_same_v<T, SRL::NpcQuestDataStruct>)
			//	{
			//		auto data = std::get<SRL::NpcQuestDataStruct>(npc_data.usage_data);
			//		if (m_PlayersState[player_id].npc_dialog_id.count(npc_id))
			//			dialog_id = m_PlayersState[player_id].npc_dialog_id[npc_id];
			//		else
			//			m_PlayersState[player_id].npc_dialog_id[npc_id] = 0;
			//		Send(sender, MSG_STC_START_DIALOG, npc_id, dialog_id);
			//	}
			//	else if constexpr (std::is_same_v<T, SRL::NpcPqDataStruct>)
			//	{
			//		if (m_PartyLeaderToMembersMap.find(player_id) != m_PartyLeaderToMembersMap.end())
			//		{
			//			m_PqGoals[m_PlayerToPartyLeaderMap[player_id]] = { {0, SRL::QuestDouble{0, 5}},  {1, SRL::QuestDouble{1, 5}} };
			//			for (const auto& player_in_party : m_PartyLeaderToMembersMap[player_id])
			//			{
			//				Send(m_ClientAddress[player_in_party], MSG_STC_CHANGE_MAP, ChangeMapMsg{ 2 });
			//				player_change_map(player_in_party, 2);
			//			}
			//			m_PartiesInPqs.push_back(player_id);
			//		}
			//	}
			//}, npc_data.usage_data
			//);


			//std::visit(overloaded{
			//	//visit_only_for<SRL::NpcQuestDataStruct, SRL::NpcPqDataStruct,
			//	// SRL::NpcMerchantDataStruct>{}, // here
			//	//[](auto arg) {  },
			//	[&](SRL::NpcQuestDataStruct arg)
			//{
			//		auto data = std::get<SRL::NpcQuestDataStruct>(npc_data.usage_data);
			//		if (m_PlayersState[player_id].npc_dialog_id.count(npc_id))
			//			dialog_id = m_PlayersState[player_id].npc_dialog_id[npc_id];
			//		else
			//			m_PlayersState[player_id].npc_dialog_id[npc_id] = 0;
			//		Send(sender, MSG_STC_START_DIALOG, npc_id, dialog_id);
			//},
			//	[&](SRL::NpcPqDataStruct arg)
			//{
			//},
			//	[&](SRL::NpcMerchantDataStruct arg) {},
			//	[&](SRL::NpcJobAdvancementDataStruct arg) {},
			//	}, npc_data.usage_data);
}
