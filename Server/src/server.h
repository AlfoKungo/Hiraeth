#pragma once
#include <queue>
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <bitset>
#include <map>
#include "address.h"
#include "socket.h"
#include "net/protocol.h"
#include "net/srl_funcs.h"
#include "net/server_funcs.h"
#include "maths/vec2.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/map.hpp"
#include "utills/a_timer.h"
#include "srl/map_data.h"
#include "mob_manager.h"
#include "db_client.h"
#include "srl/all_srl.h"
#include "net/client_funcs.h"
#include "net/net_msgs.h"
//#include "socket_handler.h"
#include "map_holder.h"
#include "item_funcs.h"
#include <type_traits>
#include "npc_logic.h"

//const int MaxClients = 64;
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

namespace hiraeth::network {
	//struct PlayerQuestData
	//{
	//	unsigned int quest_id, quest_stage;
	//};
	struct TradeDataStruct
	{
		std::map<unsigned int, SRL::EquipDbStruct> trade_items;
		bool have_accepted{ false };
	};

	class Server
	{
	private:
		int m_maxClients;
		int m_NumConnectedClients;
		//Socket m_Socket;
		std::map<char, std::function<void(Address)>> m_DistTable{};
		//std::map<char, void(Server::*)(Address)> m_DistTable2{};
		//bool m_ClientConnected[MaxClients];

		std::map<unsigned int, MapHolder> m_MapHolder;
		std::map<unsigned int, unsigned int> m_PlayerToMapId;
		std::vector<unsigned int> m_ClientsIds;

		std::map<unsigned int, PlayerHoldState> m_PlayersState;
		//unsigned int m_ItemsIdCounter{};
		//players_states
		//std::map<unsigned int, std::vector<PlayerQuestData>> m_PlayerQuestData;
		//std::map<IdType, std::map<unsigned int, unsigned int>> m_PlayerQuestData;
		std::map<unsigned int, PlayerStats> m_PlayersStats;
		std::map<unsigned int, PlayerMsgState> m_PlayersMsgs;
		std::map<unsigned int, std::vector<unsigned int>> m_PartyLeaderToMembersMap;
		std::map<unsigned int, unsigned int> m_PlayerToPartyLeaderMap;
		std::map<unsigned int, std::map<MobId, KillStruct>> m_PqGoals;
		std::vector<unsigned int> m_PartiesInPqs;
		std::map<IdType, IdType> m_TradeMap;
		std::map<IdType, TradeDataStruct> m_TradeDataMap;
		DbClient* m_DbClient;

		//BufferType m_Buffer[BufferSize];
		size_t m_RecvSize{ 0 };
		//size_t m_Size{ 0 };
		ATimer m_Timer;

		struct QueueData
		{
			char* buffer{ nullptr };
			int size{ 0 };
			Address sender{};
		};
		std::queue<QueueData> m_DataQueue;
		std::mutex m_Mutex;
		std::condition_variable m_Cv;
	public:

		Server(DbClient* db_client)
			: m_maxClients(MaxClients),
			m_NumConnectedClients(0),
			m_DbClient(db_client)
		{
			if (!m_Socket.Open(PORT))
			{
				printf("failed to create socket!\n");
				return;
			}
			bindFunctionToChar(MSG_CTS_ACK, &Server::ReceiveAck);
			bindFunctionToChar(MSG_CTS_OPEN_CONNECTION, &Server::OpenConnection);
			bindFunctionToChar(MSG_CTS_CLOSE_CONNECTION, &Server::CloseConnection);
			bindFunctionToChar(MSG_CTS_LOCATION_UPDATE, &Server::LocationUpdate);
			bindFunctionToChar(MSG_CTS_KA, &Server::KeepAlive);
			bindFunctionToChar(MSG_CTS_HIT_MOB, &Server::HitMob);
			bindFunctionToChar(MSG_CTS_NPC_CLICK, &Server::NpcClick);
			bindFunctionToChar(MSG_CTS_DIALOG_BUTTON_CLICKED, &Server::DialogButtonClicked);
			bindFunctionToChar(MSG_CTS_DIALOG_NEXT, &Server::DialogNext);
			bindFunctionToChar(MSG_CTS_SHOP_BUY_ITEM, &Server::ShopBuyItem);
			//bindFunctionToChar(MSG_CTS_ACCEPT_QUEST, &Server::AcceptQuest);
			bindFunctionToChar(MSG_CTS_FINISH_QUEST, &Server::ReceiveReward);
			bindFunctionToChar(MSG_CTS_CHAR_GOT_HIT, &Server::CharGotHit);
			bindFunctionToChar(MSG_CTS_CHAR_USE_SKILL_E, &Server::CharUseSkillE);
			bindFunctionToChar(MSG_CTS_CHAR_USE_SKILL_A, &Server::CharUseSkillA);
			bindFunctionToChar(MSG_CTS_PICK_ITEM, &Server::PickItem);
			bindFunctionToChar(MSG_CTS_INCREASE_SKILL, &Server::IncreaseSkill);
			bindFunctionToChar(MSG_CTS_WEAR_EQUIP, &Server::WearEquip);
			bindFunctionToChar(MSG_CTS_INVENTORY_ACTION, &Server::SwitchInventoryItems);
			bindFunctionToChar(MSG_CTS_PLAYER_SAY, &Server::PlayerMsg);
			bindFunctionToChar(MSG_CTS_PARTY_REQUEST, &Server::RequestParty);
			bindFunctionToChar(MSG_CTS_ACCEPT_PARTY, &Server::AcceptParty);
			bindFunctionToChar(MSG_CTS_TRADE_REQUEST, &Server::TradeRequest);
			bindFunctionToChar(MSG_CTS_TRADE_REQUEST_ACCEPT, &Server::TradeRequestAccept);
			bindFunctionToChar(MSG_CTS_TRADE_ADD_ITEM_TO_BOX, &Server::TradeAddItemToBox);
			bindFunctionToChar(MSG_CTS_TRADE_ACCEPT, &Server::TradeAccept);
			bindFunctionToChar(MSG_CTS_TRADE_CANCEL, &Server::TradeCancel);
			bindFunctionToChar(MSG_CTS_ENTER_PORTAL, &Server::EnterPortal);
			bindFunctionToChar(MSG_CTS_DROP_ITEM, &Server::DropItem);
			bindFunctionToChar(MSG_INR_MOB_HIT, &Server::InrMobGotHit);
			bindFunctionToChar(MSG_INR_MOB_UPDATE, &Server::InrMobUpdate);
			bindFunctionToChar(MSG_INR_ROUTINE_UPDATE, &Server::RoutineUpdate);

			m_MapHolder.emplace(0, 0);
			m_MapHolder.emplace(1, 1);
			for (unsigned int i = 0; i < 15; ++i)
				sendDropItem(0, m_MapHolder[0].addDrop((i % 10) % 5, (i / 5) % 2, maths::vec2(int(i - 6) * 80, 0)));
			for (unsigned int i = 0; i < 3; ++i)
				sendDropItem(0, m_MapHolder[1].addDrop((i % 10) % 5, (i / 5) % 2, maths::vec2(int(i - 6) * 80, 0)));
		}
		void main_60fps_loop();
		void main_block_receive_and_async_send();
		void main_block_threaded_queue();
		//void dataReaderCv();
		void main_60fps_threaded_queue();
		void dataReader();

		void addMessageAfterT(int time, char* buffer, int size);
		void createMessageThread(char MessageType, int milliseconds);

		void switchData(Address sender);

		unsigned int sendConnectionResponse(Address sender);
		void closeConnection(BufferType* buffer);
		void receiveLocation(BufferType* buffer);

		void bindFunctionToChar(char bytecode, void(Server::* fptr)(Address))
		{
			std::function<void(Address)> val = std::bind(fptr, this, std::placeholders::_1);
			//std::function<void(Address)> val2 = [&](Address sender) {(this->*fptr)(sender); };
			m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val));
			//m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val2));
			//m_DistTable2.insert(std::pair<char, void(Server::*)(Address)>(bytecode, fptr));
		}

		//template<typename T>
		//void Send_dynamic(unsigned char msg_id, T&&  msg)
		//{
		//	auto [data, size] = srl_dynamic_type(msg);
		//	m_Size = create_client_packet_with_buffer(m_SendBuffer, msg_id, m_Id, *data, size);
		//	Send();
		//}
		template<typename T>
		void Send_dynamic(Address sender, unsigned char msg_id, T msg)
		{
			auto [data, size] = srl_dynamic_type(msg);
			m_Size = construct_server_packet_with_buffer(m_Buffer, msg_id,
				*data, size);
			m_Socket.Send(sender, m_Buffer, m_Size);
		}
		template<typename ... Ts>
		void Send(Address sender, unsigned char msg_id, Ts&& ... dts)
		{
			m_Size = construct_server_packet(m_Buffer, msg_id, dts ...);
			m_Socket.Send(sender, m_Buffer, m_Size);
		}

		// Destined Functions
		void OpenConnection(Address sender)
		{
			auto map_id = sendConnectionResponse(sender);
			//m_MobManager.recalculateAllMobs();
			//m_MapHolder[map_id].mob_manager.recalculateAllMobs();
			m_MapHolder[map_id].sendMobsData(sender);
		}
		void CloseConnection(Address sender)
		{
			closeConnection(m_Buffer);
		}
		void LocationUpdate(Address sender)
		{
			const auto player_id = dsrl_type<unsigned int>(m_Buffer + 1);
			receiveLocation(m_Buffer);
			m_MapHolder[m_PlayerToMapId[player_id]].sendUpdateLocationToAll(sender);
			//sendUpdateLocationToAll(sender);
		}
		void KeepAlive(Address sender)
		{
			//const auto player_id = dsrl_type<unsigned int>(m_Buffer + 1);
			//m_MapHolder[m_PlayerToMapId[player_id]].sendUpdateLocationToAll(sender);
		}
		void ReceiveAck(Address sender)
		{
			auto [client_id, ack_id] = dsrl_types<unsigned int, unsigned int>(m_Buffer + 1);
			auto& player_msgs = m_PlayersMsgs[client_id].player_msgs;
			auto msg = player_msgs.find(ack_id);
			if (msg != player_msgs.end())
			{
				player_msgs.erase(msg);
			}
		}
		void HitMob(Address sender);

		void NpcClickOpenDialog(Address sender)
		{
		}

		void NpcClick(Address sender)
		{
			//auto [player_id, npc_id] = dsrl_types<IdType, unsigned int>(m_Buffer + 1);
			auto player_id = dsrl_type<IdType>(m_Buffer + 1);
			auto npc_id = dsrl_type<unsigned int>(m_Buffer + 1 + sizeof(IdType));
			auto npc_data = SRL::deserial<SRL::NpcInfo>(DF_NPC, npc_id);

			std::visit(overloaded{
				[&](SRL::NpcUsageDialog data)
			{
				unsigned int dialog_id = 0;
				if (m_PlayersState[player_id].npc_dialog_id.find(npc_id) != m_PlayersState[player_id].npc_dialog_id.end())
					dialog_id = m_PlayersState[player_id].npc_dialog_id[npc_id];
				while (true)
				{
					if ((npc_data.dialog_tree.find(dialog_id + 1) == npc_data.dialog_tree.end()) ||
						(!are_reqs_fulfilled(npc_data.dialog_tree[dialog_id + 1].reqs, m_PlayersStats[player_id],
					m_PlayersState[player_id].active_kill_quests, m_PlayersState[player_id].fulfilled_quests)))
						break;
					dialog_id++;
				}
				m_PlayersState[player_id].npc_dialog_id[npc_id] = dialog_id;
				Send(sender, MSG_STC_NPC_START_DIALOG, MsgStcNpcStartDialog{ npc_id, dialog_id});
			},
			[&](SRL::NpcUsagePq data)
			{
				if (m_PartyLeaderToMembersMap.find(player_id) != m_PartyLeaderToMembersMap.end())
				{
					// check if party members fulfil requirements
					if (m_PartyLeaderToMembersMap.size() == data.party_members_amount)
					{
						for (const auto& player_in_party : m_PartyLeaderToMembersMap[player_id])
						{
							if (!(m_PlayersStats[player_in_party].char_lvl >= data.lvl_req_min &&
								m_PlayersStats[player_in_party].char_lvl <= data.lvl_req_max))
							{
								Send(sender, MSG_STC_NPC_START_DIALOG, MsgStcNpcStartDialog{ npc_id, 1});
								break;
							}
						}

						// set party members in new map
						m_PqGoals[m_PlayerToPartyLeaderMap[player_id]] = { {0, SRL::QuestDouble{0, 5}},  {1, SRL::QuestDouble{1, 5}} };
						for (const auto& player_in_party : m_PartyLeaderToMembersMap[player_id])
						{
							Send(m_ClientAddress[player_in_party], MSG_STC_CHANGE_MAP, ChangeMapMsg{ data.pq_map_id });
							player_change_map(player_in_party, data.pq_map_id);
						}
						m_PartiesInPqs.push_back(player_id);
					}
					else
						Send(sender, MSG_STC_NPC_START_DIALOG, MsgStcNpcStartDialog{ npc_id, 1});
				}
					else
						Send(sender, MSG_STC_NPC_START_DIALOG, MsgStcNpcStartDialog{ npc_id, 1});
			},
			[&](SRL::NpcUsageMerchant data)
			{
				Send_dynamic(sender, MSG_STC_SHOP_OPEN, MsgStcShopOpen{npc_id,data.sell_items});
			},
				}, npc_data.usage_data);
		}

		void DialogButtonClicked(Address sender)
		{
			auto player_id = dsrl_type<IdType>(m_Buffer + 1);
			auto msg = dsrl_type<MsgCtsDialogButtonClick>(m_Buffer + 1 + sizeof(IdType));
			auto npc_data = SRL::deserial<SRL::NpcInfo>(DF_NPC, msg.npc_id);
			auto usage_data = std::get<SRL::NpcUsageDialog>(npc_data.usage_data);

			const auto& dialog_struct = npc_data.dialog_tree[msg.dialog_id].msgs[msg.sub_dialog_id];
			SRL::ActionVariant action;
			//auto action = dialog_struct.buttons[msg.button_id].action;
			if (dialog_struct.buttons.size() <= msg.button_id)
				action = dialog_struct.text_buttons[msg.button_id - dialog_struct.buttons.size()].action;
			else
				action = dialog_struct.buttons[msg.button_id].action;
			std::visit(overloaded{
				[&](SRL::DANextDialog data)
			{
				Send(sender, MSG_STC_DIALOG_NEXT_SUB_DIALOG, MsgStcDialogNextSubDialog{});
			},
				[&](SRL::DAAddQuest data)
			{
				AddQuest(sender, player_id, data.quest_id);
				Send(sender, MSG_STC_DIALOG_NEXT_SUB_DIALOG, MsgStcDialogNextSubDialog{});
			},
				[&](SRL::DAReceiveReward data)
			{
				m_PlayersState[player_id].npc_dialog_id[msg.npc_id]++;
				//Send(sender, MSG_STC_DIALOG_NEXT_SUB_DIALOG, MsgStcDialogNextSubDialog{msg.npc_id, msg.dialog_id, msg.sub_dialog_id + 1});
				Send(sender, MSG_STC_DIALOG_NEXT_SUB_DIALOG, MsgStcDialogNextSubDialog{});
			},
				[&](SRL::DAGoToDialog data)
			{
				Send(sender, MSG_STC_DIALOG_SET, MsgStcDialogSet{msg.npc_id, data.next_map});
			},
				[&](SRL::DAChangeMap data)
			{
				Send(sender, MSG_STC_DIALOG_NEXT_SUB_DIALOG, MsgStcDialogNextSubDialog{});
				Send(sender, MSG_STC_CHANGE_MAP, ChangeMapMsg{ data.next_map });
				player_change_map(player_id, data.next_map);
			},
				[&](SRL::DASetJob data)
			{
				auto new_job_id = 1000 * data.job_id;
				Send(sender, MSG_STC_UPDATE_JOB,MsgStcUpdateJob{new_job_id});
				Send(sender, MSG_STC_DIALOG_NEXT_SUB_DIALOG, MsgStcDialogNextSubDialog{});
				m_DbClient->setJob(player_id, new_job_id);
			},
				}, action);
		}

		void NpcClickedOpenDialog(Address sender, SRL::NpcUsageDialog dialog_tree)
		{
		}

		void DialogNext(Address sender)
		{
			//auto [client_id, npc_index, dialog_index] = dsrl_types<IdType, unsigned int, unsigned int>(m_Buffer + 1);
			//Send(sender, MSG_STC_DIALOG_NEXT_ANSWER, dialog_index + 1);
		}

		void ShopBuyItem(Address sender)
		{
			auto player_id = dsrl_type<IdType>(m_Buffer + 1);
			auto msg = dsrl_type<MsgCtsShopBuyItem>(m_Buffer + 1 + sizeof(IdType));
			auto npc_data = SRL::deserial<SRL::NpcInfo>(DF_NPC, msg.npc_id);
			auto shop_data = std::get<SRL::NpcUsageMerchant>(npc_data.usage_data);
			auto [item_tab, item_type_id, item_price] = shop_data.sell_items.at(msg.item_num);

			// reduce money from player
			auto new_money = m_DbClient->reduceMoney(player_id, item_price);
			Send(sender, MSG_STC_UPDATE_MONEY, MsgStcUpateMoney{ new_money });

			// update item for player and db, unite this so one function
			unsigned int item_loc = 0;
			if (item_tab == network::USE_ITEM)
			{
				item_loc = m_DbClient->addItem(item_tab, player_id, item_type_id);
				Send(sender, MSG_STC_ADD_ITEM,
					item_tab, item_loc, item_type_id);
			}
			else if (item_tab == network::EQUIP_ITEM)
			{
				auto equip_info = CreateBasicEquip(item_type_id);
				auto equip_db = SRL::EquipDbStruct{ item_type_id, equip_info };
				item_loc = m_DbClient->addEquipInv(player_id, equip_db);
				Send_dynamic(sender, MSG_STC_ADD_EQUIP_ITEM, AddEquipItemMsg{ item_loc, item_type_id, equip_db });
			}
		}

		void AddQuest(Address sender, unsigned int player_id, unsigned int quest_id)
		{
			auto quest_data = SRL::deserial<SRL::QuestData>(DF_QUEST, quest_id);

			for (auto prop : quest_data.quest_properties)
			{
				if (prop.key == SRL::KillAmount)
				{
					auto quest_double = std::get<SRL::QuestDouble>(prop.value);
					m_PlayersState[player_id].active_kill_quests[quest_id][quest_double.type] = KillStruct{ quest_double };
				}
			}
			//m_PlayersState[player_id].npc_dialog_id[npc_index]++;
			m_DbClient->setQuestAsActive(player_id, quest_id);
			msgStcSetQuestIp msg_data{ m_PlayersMsgs[player_id].get_ack_id(), quest_id };
			m_Size = construct_server_packet(m_Buffer, MSG_STC_SET_QUEST_IP, msg_data);
			m_PlayersMsgs[player_id].add_new_msg(m_Buffer, m_Size, msg_data.ack_id);

			m_Socket.Send(sender, m_Buffer, m_Size);
		}
		void AcceptQuest(Address sender, unsigned int player_id, unsigned int npc_index, unsigned int quest_id)
		{
			//auto [client_id, npc_index, dialog_index] = dsrl_types<IdType, unsigned int, unsigned int>(m_Buffer + 1);
			//std::map<unsigned int, unsigned int> npc_n_did_to_quest{ {2,0},{3,1} };
			//unsigned int key = (npc_index + 1) * 2 + dialog_index;
			//unsigned int quest_id = npc_n_did_to_quest[key];
			auto quest_data = SRL::deserial<SRL::QuestData>(DF_QUEST, quest_id);

			for (auto prop : quest_data.quest_properties)
			{
				if (prop.key == SRL::KillAmount)
				{
					auto quest_double = std::get<SRL::QuestDouble>(prop.value);
					m_PlayersState[player_id].active_kill_quests[quest_id][quest_double.type] = KillStruct{ quest_double };
					//m_PlayersState[client_id].active_kill_quests[quest_id][quest_double.type] = KillStruct{0,0};
				}
			}
			m_PlayersState[player_id].npc_dialog_id[npc_index]++;
			//if (npc_id == 1)
			//{
			//	if (m_PartyLeaderToMembersMap.find(player_id) != m_PartyLeaderToMembersMap.end())
			//	{
			//		m_PqGoals[m_PlayerToPartyLeaderMap[player_id]] = { {0, SRL::QuestDouble{0, 5}},  {1, SRL::QuestDouble{1, 5}} };
			//		for (const auto& player_in_party : m_PartyLeaderToMembersMap[player_id])
			//		{
			//			//m_Size = construct_server_packet(m_Buffer, MSG_STC_CHANGE_MAP, ChangeMapMsg{ 2 });
			//			//m_Socket.Send(m_ClientAddress[player_in_party], m_Buffer, m_Size);
			//			Send(m_ClientAddress[player_in_party], MSG_STC_CHANGE_MAP, ChangeMapMsg{ 2 });
			//			player_change_map(player_in_party, 2);
			//		}
			//		m_PartiesInPqs.push_back(player_id);
			//	}
			//}
			//else
			//if (m_PlayerQuestData.find(client_id) == m_PlayerQuestData.end())
			//	m_PlayerQuestData[client_id] = std::map<unsigned int, unsigned int>();
			//if (m_PlayerQuestData[client_id].find(npc_index) == m_PlayerQuestData[client_id].end())
			//	m_PlayerQuestData[client_id][npc_index] = 0;
			//m_PlayerQuestData[client_id][npc_index] += 1;
			m_DbClient->setQuestAsActive(player_id, quest_id);
			msgStcSetQuestIp msg_data{ m_PlayersMsgs[player_id].get_ack_id(), quest_id };
			//msgStcSetQuestIp msg_data{ 3, 4 };
			m_Size = construct_server_packet(m_Buffer, MSG_STC_SET_QUEST_IP, msg_data);
			m_PlayersMsgs[player_id].add_new_msg(m_Buffer, m_Size, msg_data.ack_id);

			m_Socket.Send(sender, m_Buffer, m_Size);
		}
		void ReceiveReward(Address sender)
		{
			auto [client_id, npc_index, dialog_index] = dsrl_types<IdType, unsigned int, unsigned int>(m_Buffer + 1);
			std::map<unsigned int, unsigned int> npc_n_did_to_quest{ {2,0},{3,1} };
			unsigned int key = (npc_index + 1) * 2 + dialog_index;
			unsigned int quest_id = npc_n_did_to_quest[key];
			auto quest_data = SRL::deserial<SRL::QuestData>(DF_QUEST, quest_id);
			FinishQuestMsg rm{};
			rm.quest_id = quest_id;
			for (const auto& prop : quest_data.quest_properties)
			{
				switch (prop.key)
				{
				case SRL::QuestProps::ExpReward:
					rm.exp = unsigned int(std::get<int>(prop.value));
					break;
				default:
					break;
				}
			}
			auto [data, size] = srl_dynamic_type(rm);
			m_Size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_FINISH_QUEST, *data, size);
			m_Socket.Send(sender, m_Buffer, m_Size);
		}
		void CharGotHit(Address sender)
		{
			auto [client_id, new_hp] = dsrl_types<IdType, unsigned int>(m_Buffer + 1);
			m_DbClient->setValue(client_id, "hp", new_hp);
		}
		void CharUseSkillE(Address sender)
		{
			auto [player_id, skill_id, new_mp] = dsrl_types<IdType, unsigned int, unsigned int>(m_Buffer + 1);
			m_DbClient->setValue(player_id, "mp", new_mp);
			m_Size = construct_server_packet(m_Buffer, MSG_STC_CHAR_USE_SKILL_E, player_id, skill_id);
			sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], m_Size, player_id);
		}
		void CharUseSkillA(Address sender)
		{
			auto [player_id] = dsrl_types<IdType>(m_Buffer + 1);
			const auto attack_skill_msg = dsrl_dynamic_type<AttackSkillMsg>(m_Buffer + 5);
			CharAttackSkillMsg char_attack_msg{ player_id, attack_skill_msg };
			auto [data, size] = srl_dynamic_type(char_attack_msg);
			m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_CHAR_USE_SKILL_A, *data, size);
			sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], m_Size, player_id);
			auto map_id = m_PlayerToMapId[player_id];
			auto& mob_manager = m_MapHolder[map_id].mob_manager;

			for (const auto& monster_hit : attack_skill_msg.monsters_hit)
				if (mob_manager.damageMob(monster_hit))
				{
					MobDied(map_id, player_id, monster_hit.monster_id);
				}
		}
		void MobDied(unsigned int map_id, unsigned int player_id, unsigned int mob_id)
		{
			//UpdateQuestOnKill(player_id, mob_id);
			//auto dead_pos = m_MobManager.monsterDied(monster_id);
			auto& map_holder = m_MapHolder[map_id];
			auto dead_pos = m_MapHolder[map_id].mob_manager.monsterDied(mob_id);
			//auto [inv_items, use_items, etc_items] = calculate_drops(mob_id);
			auto mob_type_id = map_holder.mob_manager.m_Monsters[mob_id].monster_type;
			auto drops_vec = calculate_drops(mob_type_id);
			UpdateQuestOnKill(player_id, mob_type_id);
			std::vector<ItemDropData> drops;
			for (const auto drop : std::get<0>(drops_vec))
				drops.push_back(map_holder.addDrop(drop, 0, dead_pos));
			for (const auto drop : std::get<1>(drops_vec))
				drops.push_back(map_holder.addDrop(drop, 1, dead_pos));
			//for (const auto drop : std::get<2>(drops_vec))
			//	drops.push_back(map_holder.addItem(drop, 2, dead_pos));
			auto [data, size] = srl_dynamic_type(MonsterDiedMsg{ mob_id, drops });
			m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_MOB_DIED, *data, size);
			sendDataToAllClientsInMap(map_id, m_Size);
			unsigned int exp = 30;
			if (m_PlayerToPartyLeaderMap.find(player_id) != m_PlayerToPartyLeaderMap.end())
			{
				auto party_leader = m_PlayerToPartyLeaderMap[player_id];
				auto amount_of_members = m_PartyLeaderToMembersMap[party_leader].size();
				std::vector<std::tuple<double, double>> splits{ {0.85, 0.2}, {0.8, 0.15}, {0.75,0.14 }, {0.7, 0.13}, {0.65, 0.12} };
				auto [killer_s, party_s] = splits[amount_of_members - 2];
				unsigned int killer_split{ unsigned int(exp * killer_s) }, party_split{ unsigned int(exp * party_s) };
				for (const auto& party_member : m_PartyLeaderToMembersMap[party_leader])
				{
					if (party_member == player_id)
					{
						Send(m_ClientAddress[party_member], MSG_STC_RECEIVED_EXP, killer_split);
					}
					else
					{
						Send(m_ClientAddress[party_member], MSG_STC_RECEIVED_EXP, party_split);
					}
				}
				//if (std::find(m_PartiesInPqs.begin(), m_PartiesInPqs.end(), party_leader) != m_PartiesInPqs.end())
				if (m_PqGoals.find(party_leader) != m_PqGoals.end())
				{
					auto& goals = m_PqGoals[party_leader];
					if (goals.find(mob_type_id) != goals.end())
					{
						goals[mob_type_id].kill_amount++;
					}
				}
			}
			else
			{
				//m_Size = construct_server_packet(m_Buffer, MSG_STC_RECEIVED_EXP, exp);
				//m_Socket.Send(m_ClientAddress[player_id], m_Buffer, m_Size);
				Send(m_ClientAddress[player_id], MSG_STC_RECEIVED_EXP, exp);
			}
			//if (map_id == 2)
		}
		void PickItem(Address sender)
		{
			auto [player_id, item_id] = dsrl_types<IdType, unsigned int>(m_Buffer + 1);
			auto player_map_id = m_PlayerToMapId[player_id];
			auto& map_holder = m_MapHolder[player_map_id];
			auto& items_dropped = map_holder.items_dropped;
			auto item_picked = items_dropped[item_id];
			unsigned int item_loc = 0;
			if (item_picked.item_tab == network::USE_ITEM)
			{
				item_loc = m_DbClient->addItem(item_picked.item_tab, player_id, item_picked.item_type_id);
				Send(sender, MSG_STC_ADD_ITEM,
					item_picked.item_tab, item_loc, item_picked.item_type_id);
			}
			else if (item_picked.item_tab == network::EQUIP_ITEM)
			{
				auto equip_info = map_holder.m_EquipsStats[item_id];
				auto equip_db = SRL::EquipDbStruct{ item_picked.item_type_id, equip_info };
				item_loc = m_DbClient->addEquipInv(player_id, equip_db);
				Send_dynamic(sender, MSG_STC_ADD_EQUIP_ITEM, AddEquipItemMsg{ item_loc, item_picked.item_type_id, equip_db });
				map_holder.m_EquipsStats.erase(item_id);
			}
			items_dropped.erase(item_id);

			m_Size = construct_server_packet(m_Buffer, MSG_STC_PICK_ITEM,
				PickItemMsg{ player_id, item_id });
			sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], m_Size, player_id);
			//sendDataToAllClients(m_Size);
		}
		void IncreaseSkill(Address sender)
		{
			auto [client_id, skill_id] = dsrl_types<IdType, unsigned int>(m_Buffer + 1);
			m_DbClient->increaseSkillPoints(client_id, skill_id);
			Send(sender, MSG_STC_INCREASE_SKILL, skill_id);
		}
		void WearEquip(Address sender)
		{
			auto [player_id, equip_type, equip_loc] = dsrl_types<IdType, SRL::EquipItemType, unsigned int>(m_Buffer + 1);
			auto& map_holder = m_MapHolder[m_PlayerToMapId[player_id]];
			auto amount_before = map_holder.players_equips[player_id].size();
			auto char_equips = m_DbClient->wearEquip(player_id, equip_type, equip_loc);
			map_holder.players_equips[player_id] = char_equips;

			map_holder.sendWearUpdate(player_id, char_equips);
		}
		void SwitchInventoryItems(Address sender)
		{
			auto [client_id, item_loc1, item_loc2, tab_index] =
				dsrl_types<IdType, unsigned int, unsigned int, unsigned int>(m_Buffer + 1);
			//if (tab_index == 0)
			m_DbClient->switchInventoryItems(client_id, item_loc1, item_loc2, tab_index);
			//else if (tab_index == 1)
			//	m_DbClient->switchInventoryItems(client_id, item_loc1, item_loc2, tab_index);
		}
		void PlayerMsg(Address sender)
		{
			auto [player_id] =
				dsrl_types<IdType>(m_Buffer + 1);
			auto msg = dsrl_dynamic_type<std::string>(m_Buffer + 5);
			auto [data, size] = srl_dynamic_type(PlayerSayMsg{ player_id, msg });
			m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_PLAYER_SAY,
				*data, size);
			sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], m_Size, player_id);
		}
		void RequestParty(Address sender)
		{
			auto [client_id, player_to_invite] = dsrl_types<IdType, unsigned int>(m_Buffer + 1);
			//m_Size = construct_server_packet(m_Buffer, MSG_STC_PARTY_REQUEST, client_id);
			//m_Socket.Send(m_ClientAddress[player_to_invite], m_Buffer, m_Size);
			Send(m_ClientAddress[player_to_invite], MSG_STC_PARTY_REQUEST, client_id);
		}
		void AcceptParty(Address sender)
		{
			auto [new_party_member, party_leader] = dsrl_types<unsigned int, unsigned int>(m_Buffer + 1);
			if (m_PartyLeaderToMembersMap.find(party_leader) == m_PartyLeaderToMembersMap.end())
			{
				m_PartyLeaderToMembersMap[party_leader] = { party_leader };
				m_PlayerToPartyLeaderMap[party_leader] = party_leader;
			}
			m_PlayerToPartyLeaderMap[new_party_member] = party_leader;
			m_PartyLeaderToMembersMap[party_leader].push_back(new_party_member);
			auto msg = PartyUpdateMsg{ {} , 0 };
			for (const auto& party_member : m_PartyLeaderToMembersMap[party_leader])
			{
				msg.party_members.push_back({ party_member, m_PlayersStats[party_member].name });
			}
			auto [data, size] = srl_dynamic_type(msg);
			for (const auto& party_member : m_PartyLeaderToMembersMap[party_leader])
			{
				m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_UPDATE_PARTY_STATE, *data, size);
				m_Socket.Send(m_ClientAddress[party_member], m_Buffer, m_Size);
			}
		}
		void TradeRequest(Address sender)
		{
			auto [player_id, msg] = dsrl_types<IdType, MsgCtsTradeRequest>(m_Buffer + 1);
			Send(m_ClientAddress[msg.player_to_trade], MSG_STC_TRADE_REQUEST, player_id);
		}
		void TradeRequestAccept(Address sender)
		{
			auto [player_id, msg] = dsrl_types<IdType, MsgCtsTradeRequestAccept>(m_Buffer + 1);
			Send(m_ClientAddress[msg.player_who_wants_to_trade], MSG_STC_TRADE_FACILITATE,
				MsgStcTradeFacilitate{ player_id });
			Send(m_ClientAddress[player_id], MSG_STC_TRADE_FACILITATE,
				MsgStcTradeFacilitate{ msg.player_who_wants_to_trade });
			m_TradeMap[player_id] = msg.player_who_wants_to_trade;
			m_TradeMap[msg.player_who_wants_to_trade] = player_id;
		}
		void TradeAddItemToBox(Address sender)
		{
			auto [player_id, msg] = dsrl_types<IdType, MsgCtsTradeAddItemToBox>(m_Buffer + 1);
			auto inv = m_DbClient->getDynamicType<decltype(PlayerData::inv_equip)>(player_id, INV_EQUIP);
			m_TradeDataMap[player_id].trade_items[msg.trade_index] = inv[msg.inventory_index];
			//m_DbClient->deleteInventoryItem<decltype(PlayerData::inv_use)::value_type::second_type>(player_id, msg.inventory_index, EQUIP_ITEM);
			auto item = m_DbClient->deleteInventoryItem<decltype(PlayerData::inv_equip)::value_type::second_type>(player_id, msg.inventory_index, EQUIP_ITEM);
			{
				auto [data, size] = srl_dynamic_type(MsgStcTradeAddItemToBox{ msg.trade_index, inv[msg.inventory_index] , true });
				m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_TRADE_ADD_ITEM_TO_BOX,
					*data, size);
				m_Socket.Send(sender, m_Buffer, m_Size);
			}
			{
				auto [data, size] = srl_dynamic_type(MsgStcTradeAddItemToBox{ msg.trade_index, inv[msg.inventory_index] , false });
				m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_TRADE_ADD_ITEM_TO_BOX,
					*data, size);
				m_Socket.Send(m_ClientAddress[m_TradeMap[player_id]], m_Buffer, m_Size);
			}
		}
		void TradeAccept(Address sender)
		{
			auto [player_id] = dsrl_types<IdType>(m_Buffer + 1);
			if (!m_TradeDataMap[player_id].have_accepted)
			{
				m_TradeDataMap[player_id].have_accepted = true;
				auto other_player = m_TradeMap[player_id];
				if (m_TradeDataMap[other_player].have_accepted)
				{
					// finalize trade
					Send(sender, MSG_STC_TRADE_FINALIZE, 0);
					Send(m_ClientAddress[other_player], MSG_STC_TRADE_FINALIZE, 0);
					give_items_to_player(other_player, m_TradeDataMap[player_id].trade_items);
					give_items_to_player(player_id, m_TradeDataMap[other_player].trade_items);
					m_TradeMap.erase(player_id);
					m_TradeMap.erase(other_player);
					m_TradeDataMap.erase(player_id);
					m_TradeDataMap.erase(other_player);
				}
				else
				{
					// send to other player
					Send(sender, MSG_STC_TRADE_PLAYER_ACCEPTED, MsgStcTradePlayerAccepted{ true });
					Send(m_ClientAddress[other_player], MSG_STC_TRADE_PLAYER_ACCEPTED, MsgStcTradePlayerAccepted{ false });
				}
			}
		}
		void give_items_to_player(IdType player_id, std::map<unsigned int, SRL::EquipDbStruct>& items)
		{
			for (const auto& [key, item] : items)
			{
				auto equip_db = item;
				auto item_loc = m_DbClient->addEquipInv(player_id, equip_db);
				auto msg = AddEquipItemMsg{ item_loc, EQUIP_ITEM, equip_db };
				Send_dynamic(m_ClientAddress[player_id], MSG_STC_ADD_EQUIP_ITEM, msg);
			}
		}
		void TradeCancel(Address sender)
		{
			auto [player_id] = dsrl_types<IdType>(m_Buffer + 1);
			auto other_player = m_TradeMap[player_id];
			Send(sender, MSG_STC_TRADE_CANCEL, 0);
			Send(m_ClientAddress[other_player], MSG_STC_TRADE_CANCEL, 0);
			give_items_to_player(player_id, m_TradeDataMap[player_id].trade_items);
			give_items_to_player(other_player, m_TradeDataMap[other_player].trade_items);
			m_TradeMap.erase(player_id);
			m_TradeMap.erase(other_player);
			m_TradeDataMap.erase(player_id);
			m_TradeDataMap.erase(other_player);
		}
		void EnterPortal(Address sender)
		{
			auto [player_id, msg] = dsrl_types<IdType, EnterPortalMsg>(m_Buffer + 1);
			if (m_PlayerToMapId[player_id] == 1)
				msg.next_map = 0;
			Send(sender, MSG_STC_ENTER_PORTAL, msg);

			// change map
			player_change_map(player_id, msg.next_map);
		}
		void DropItem(Address sender)
		{
			auto [player_id, item_loc, tab_index] =
				dsrl_types<IdType, unsigned int, unsigned int>(m_Buffer + 1);
			auto map_id = m_PlayerToMapId[player_id];
			auto& map_holder = m_MapHolder[map_id];
			auto pos = map_holder.players_state[player_id].pos;
			if (tab_index == network::EQUIP_ITEM)
			{
				auto item = m_DbClient->deleteInventoryItem<decltype(PlayerData::inv_equip)::value_type::second_type>(player_id, item_loc, tab_index);
				auto pos = map_holder.players_state[player_id].pos;
				auto item_id = map_holder.addDrop({ item.equip_id, item.equip_item_properties }, pos);
				m_Size = construct_server_packet(m_Buffer, MSG_STC_DROP_ITEM,
					MsgStcDropItem{ tab_index, item.equip_id, item_id.item_id, pos });
			}
			else
			{
				auto item = m_DbClient->deleteInventoryItem<decltype(PlayerData::inv_use)::value_type::second_type>(player_id, item_loc, tab_index);
				auto item_id = map_holder.addDrop(item.item_type_id, tab_index, map_holder.players_state[player_id].pos);
				m_Size = construct_server_packet(m_Buffer, MSG_STC_DROP_ITEM,
					MsgStcDropItem{ tab_index, item.item_type_id, item_id.item_id, pos });
			}

			sendDataToAllClientsInMap(map_id, m_Size);
		}
		void player_change_map(unsigned int player_id, unsigned int next_map_id)
		{
			if (m_MapHolder.find(next_map_id) == m_MapHolder.end())
				m_MapHolder.emplace(next_map_id, next_map_id);
			auto prev_map_id = m_PlayerToMapId[player_id];
			auto& prev_map = m_MapHolder[prev_map_id];
			auto& next_map = m_MapHolder[next_map_id];
			auto player_address = m_ClientAddress[player_id];
			next_map.players_state[player_id] = m_MapHolder[prev_map_id].players_state[player_id];
			prev_map.players_state.erase(player_id);
			m_PlayerToMapId[player_id] = next_map_id;
			prev_map.sendPlayerLeftMap(player_id);
			//m_MapHolder[next_map].sendMobsUpdate();
			sendDroppedItems(next_map_id, player_address);
			next_map.sendMobsData(player_address);

			// erase eequips data
			next_map.players_equips[player_id] = prev_map.players_equips[player_id];
			prev_map.players_equips.erase(player_id);
		}
		void InrMobGotHit(Address sender)
		{
		}
		void InrMobUpdate(Address sender)
		{
			for (auto& map_holder : m_MapHolder)
			{
				map_holder.second.updateMobManager();
			}
			//updateMobManager();
			createMessageThread(MSG_INR_MOB_UPDATE, 1000);
		}
		void sendDropItem(unsigned int map_id, ItemDropData item_drop)
		{
			//m_ItemsDropped[item_drop.item_type_id] = item_drop;
			m_Size = construct_server_packet(m_Buffer, MSG_STC_DROP_ITEM,
				MsgStcDropItem{ item_drop.item_tab, item_drop.item_type_id,item_drop.item_id,item_drop.location });

			sendDataToAllClientsInMap(map_id, m_Size);
		}
		void sendDroppedItems(unsigned int map_id, Address sender)
		{
			std::vector<ItemDropData> dropped_items;
			//for (auto di : m_ItemsDropped)
			for (auto di : m_MapHolder[map_id].items_dropped)
				dropped_items.push_back(di.second);
			auto [data, size] = srl_dynamic_type(dropped_items);
			m_Size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_DROPPED_ITEMS, *data, size);
			m_Socket.Send(sender, m_Buffer, m_Size);
		}
		void RoutineUpdate(Address sender)
		{
			for (auto& map_holder : m_MapHolder)
				map_holder.second.findExpiredItems();
			createMessageThread(MSG_INR_ROUTINE_UPDATE, 1000);
			for (auto& [client_id, msgs] : m_PlayersMsgs)
			{
				for (auto& [ack_id, msg] : msgs.player_msgs)
				{
					//m_Socket.Send(sender, msg.msg_buffer, msg.m_Size);
					m_Socket.Send(m_ClientAddress[client_id], msg.msg_buffer, msg.buffer_size);
				}
			}
		}

		bool UpdateQuestOnKill(unsigned int client_id, unsigned int mob_id)
		{
			for (auto& [quest_id, quest_props] : m_PlayersState[client_id].active_kill_quests)
				for (auto& [quest_mob_id, kill_struct] : quest_props)
					if (quest_mob_id == mob_id)
					{
						if (!kill_struct.checkIfAccomplished())
						{
							kill_struct.kill_amount++;
							m_DbClient->setByteArray(client_id, PLAYER_STATE, m_PlayersState[client_id]);
							if (checkIfQuestDone(client_id, quest_id))
								setQuestAsFinished(client_id, quest_id);
							return true;
						}
					}
			return false;
		}

		bool checkIfQuestDone(unsigned int client_id, unsigned int quest_id)
		{
			auto quest_data = m_PlayersState[client_id].active_kill_quests[quest_id];
			for (const auto& [mob_id, goal] : quest_data)
				if (!goal.checkIfAccomplished())
					return false;
			return true;
		}

		void setQuestAsFinished(unsigned int client_id, unsigned int quest_id)
		{
			//m_PlayersState[client_id].npc_dialog_id[quest_id / 2]++;
			m_PlayersState[client_id].fulfilled_quests.push_back(quest_id);
		}

		//void sendKeepAliveAnswer(Address sender)
		//{
		//	printf("ip is : %d %s %s and port is : %d\n", sender.GetAddress(),
		//		std::bitset<32>(sender.GetAddress()).to_string().c_str(),
		//		sender.GetAddressString().c_str(), sender.GetPort());
		//	const char data[] = "ack";
		//	m_Socket.Send(sender, data, sizeof(data));
		//}

		int findFreeClientIndex() const
		{
			return 2;
			//for (int i = 0; i < m_maxClients; ++i)
			//{
			//	//if (!m_ClientConnected[i])
			//	if (!isClientConnected(i))
			//		return i;
			//}
			//return -1;
		}
		unsigned int findExistingClientIndex(const Address& address) const
		{
			for (int i = 0; i < m_maxClients; ++i)
			{
				//if (m_clientConnected[i] && m_clientAddress[i] == address)
				if (m_ClientAddress[i].GetAddress() == address.GetAddress()
					&& m_ClientAddress[i].GetPort() == address.GetPort())
					return i;
			}
			return -1;
		}
		void sendDataToAllClientsInMap(unsigned int map_id, unsigned int size)
		{
			for (const auto& [player_id, _ir] : m_MapHolder[map_id].players_state)
				m_Socket.Send(m_ClientAddress[player_id], m_Buffer, size);
		}
		void sendDataToAllClientsInMapExcept(unsigned int map_id, unsigned int size, unsigned int exclude_id)
		{
			for (const auto& [player_id, _ir] : m_MapHolder[map_id].players_state)
				if (player_id != exclude_id)
					m_Socket.Send(m_ClientAddress[player_id], m_Buffer, size);
		}
		bool isClientConnected(int client_id) const
		{
			//return m_ClientConnected[clientIndex];
			return std::find(m_ClientsIds.begin(), m_ClientsIds.end(), client_id) != m_ClientsIds.end();
		}
		const Address& GetClientAddress(int clientIndex) const
		{
			return m_ClientAddress[clientIndex];
		}
	};
}