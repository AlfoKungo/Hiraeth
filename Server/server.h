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
#include "net/client_funcs.h"
#include "net/net_msgs.h"
#include "socket_handler.h"
#include "map_holder.h"

//const int MaxClients = 64;
using IdType = unsigned int;
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {
		//struct Summoner
		//{
		//	SRL::Summon summon;
		//	float summonTime;
		//};

		//static Address m_ClientAddress[MaxClients];
		//static BufferType m_Buffer[BufferSize];
		//static size_t m_Size{ 0 };


		//struct PlayerQuestData
		//{
		//	unsigned int quest_id, quest_stage;
		//};
		class Server
		{
		private:
			int m_maxClients;
			int m_NumConnectedClients;
			 //Socket m_Socket;
			std::map<char, std::function<void(Address)>> m_DistTable{};
			//std::map<char, void(Server::*)(Address)> m_DistTable2{};
			//bool m_ClientConnected[MaxClients];
			//Socket SocketHandler::m_Socket;
			//Address SocketHandler::m_ClientAddress[MaxClients];
			//BufferType SocketHandler::m_Buffer[BufferSize];
			//size_t SocketHandler::m_Size{ 0 };

			std::map<unsigned int, MapHolder> m_MapHolder;
			std::map<unsigned int, unsigned int> m_PlayerToMapId;
			std::vector<unsigned int> m_ClientsIds;
			// map_state
			//std::map<unsigned int, PlayerStateUpdateMsg> m_ClientsState;
			//std::map<unsigned int, ItemDropMsg> m_ItemsDropped;

			////MobManager m_MobManager;
			////std::queue<ItemExpirer> m_ExpiringQueue;
			std::map<unsigned int, PlayerHoldState> m_PlayersState;
			//unsigned int m_ItemsIdCounter{};
			//players_states
			//std::map<unsigned int, std::vector<PlayerQuestData>> m_PlayerQuestData;
			std::map<unsigned int, std::map<unsigned int, unsigned int>> m_PlayerQuestData;
			std::map<unsigned int, PlayerStats> m_PlayersStats;
			std::map<unsigned int, PlayerMsgState> m_PlayersMsgs;
			std::map<unsigned int, std::vector<unsigned int>> m_PartyLeaderToMembersMap;
			std::map<unsigned int, unsigned int> m_PlayerToPartyLeaderMap;
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
				//m_ClientConnected{ false },
				//m_MobManager{ 0 },
				m_DbClient(db_client)
			{
				if (!SocketHandler::m_Socket.Open(PORT))
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
				bindFunctionToChar(MSG_CTS_DIALOG_NEXT, &Server::DialogNext);
				bindFunctionToChar(MSG_CTS_ACCEPT_QUEST, &Server::AcceptQuest);
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
				bindFunctionToChar(MSG_CTS_ENTER_PORTAL, &Server::EnterPortal);
				bindFunctionToChar(MSG_INR_MOB_HIT, &Server::InrMobGotHit);
				bindFunctionToChar(MSG_INR_MOB_UPDATE, &Server::InrMobUpdate);
				bindFunctionToChar(MSG_INR_ROUTINE_UPDATE, &Server::RoutineUpdate);

				//m_MapHolder.emplace( (unsigned int)0, &m_Socket );
				//m_MapHolder.insert( std::make_pair(unsigned int(0), MapHolder{ &m_Socket } ));

				for (unsigned int i = 0; i < 15; ++i)
					//sendDropItem(addItem((i % 10) % 5, (i / 5) % 2, maths::vec2(int(i - 6) * 80, 0)));
					sendDropItem(0, m_MapHolder[0].addItem((i % 10) % 5, (i / 5) % 2, maths::vec2(int(i - 6) * 80, 0)));
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
			//void sendNewPlayerInMap(unsigned int new_char_index);
			void closeConnection(BufferType* buffer);
			void receiveLocation(BufferType* buffer);
			//void sendUpdateLocationToAll(Address sender);
			//void sendMobsData(Address sender);
			//void sendMobsUpdate(unsigned int mob_id, MobMoveCommand mmc);
			//void updateMobManager();

			void bindFunctionToChar(char bytecode, void(Server::* fptr)(Address))
			{
				std::function<void(Address)> val = std::bind(fptr, this, std::placeholders::_1);
				//std::function<void(Address)> val2 = [&](Address sender) {(this->*fptr)(sender); };
				m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val));
				//m_DistTable.insert(std::pair<char, std::function<void(Address)>>(bytecode, val2));
				//m_DistTable2.insert(std::pair<char, void(Server::*)(Address)>(bytecode, fptr));
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
				closeConnection(SocketHandler::m_Buffer);
			}
			void LocationUpdate(Address sender)
			{
				const auto player_id = dsrl_type<unsigned int>(SocketHandler::m_Buffer + 1);
				receiveLocation(SocketHandler::m_Buffer);
				m_MapHolder[m_PlayerToMapId[player_id]].sendUpdateLocationToAll(sender);
				//sendUpdateLocationToAll(sender);
			}
			void KeepAlive(Address sender)
			{
				
				//const auto player_id = dsrl_type<unsigned int>(SocketHandler::m_Buffer + 1);
				//m_MapHolder[m_PlayerToMapId[player_id]].sendUpdateLocationToAll(sender);
			}
			void ReceiveAck(Address sender)
			{
				auto [client_id, ack_id] = dsrl_types<unsigned int, unsigned int>(SocketHandler::m_Buffer + 1);
				auto& player_msgs = m_PlayersMsgs[client_id].player_msgs;
				auto msg = player_msgs.find(ack_id);
				if (msg != player_msgs.end())
				{
					player_msgs.erase(msg);
				}
			}
			void HitMob(Address sender);
			//ItemDropMsg addItem(unsigned int map_id, unsigned int item_type_id, unsigned int item_kind, maths::vec2 pos)
			//{
			//	auto new_item = ItemDropMsg{ m_ItemsIdCounter ,item_type_id, item_kind, pos };
			//	m_MapHolder[map_id].items_dropped[]
			//	m_ItemsDropped[m_ItemsIdCounter] = new_item;
			//	m_ExpiringQueue.push(ItemExpirer{ m_ItemsIdCounter, ATimer{10.0f } });
			//	m_ItemsIdCounter++;
			//	return new_item;
			//}
			//void findExpiredItems()
			//{
			//	while (!m_ExpiringQueue.empty())
			//	{
			//		if (m_ExpiringQueue.front().expire_timer.hasExpired())
			//		{
			//			if (m_ItemsDropped.find(m_ExpiringQueue.front().item_id) != m_ItemsDropped.end())
			//				sendItemExpired(m_ExpiringQueue.front().item_id);
			//			m_ExpiringQueue.pop();
			//		}
			//		else
			//			return;
			//	}
			//}
			void NpcClick(Address sender)
			{
				auto [client_id, npc_index] = dsrl_types<IdType, unsigned int>(SocketHandler::m_Buffer + 1);
				unsigned int dialog_id{ 0 };
				if (npc_index != 1)
				{
					if (m_PlayersState[client_id].npc_dialog_id.count(npc_index))
						dialog_id = m_PlayersState[client_id].npc_dialog_id[npc_index];
					else
						m_PlayersState[client_id].npc_dialog_id[npc_index] = 0;
					SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_START_DIALOG, npc_index, dialog_id);
					SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
				}
				else
				{
					SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_CHANGE_MAP, ChangeMapMsg{ 2 });
					SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
				}
			}
			void DialogNext(Address sender)
			{
				auto [client_id, npc_index, dialog_index] = dsrl_types<IdType, unsigned int, unsigned int>(SocketHandler::m_Buffer + 1);
				//unsigned int client_id, npc_index, dialog_index;
				//dsrl_types(m_Buffer, client_id, npc_index, dialog_index);
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_DIALOG_NEXT_ANSWER, dialog_index + 1);
				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
			}
			void AcceptQuest(Address sender)
			{
				auto [client_id, npc_index, dialog_index] = dsrl_types<IdType, unsigned int, unsigned int>(SocketHandler::m_Buffer + 1);
				std::map<unsigned int, unsigned int> npc_n_did_to_quest{ {2,0},{3,1} };
				unsigned int key = (npc_index + 1) * 2 + dialog_index;
				unsigned int quest_id = npc_n_did_to_quest[key];
				auto quest_data = SRL::deserial<SRL::QuestData>(DF_QUEST, quest_id);

				for (auto prop : quest_data.quest_properties)
				{
					if (prop.key == SRL::KillAmount)
					{
						auto quest_double = std::get<SRL::QuestDouble>(prop.value);
						m_PlayersState[client_id].active_kill_quests[quest_id][quest_double.type] = KillStruct{ quest_double };
						//m_PlayersState[client_id].active_kill_quests[quest_id][quest_double.type] = KillStruct{0,0};
					}
				}
				m_PlayersState[client_id].npc_dialog_id[npc_index]++;
				//if (m_PlayerQuestData.find(client_id) == m_PlayerQuestData.end())
				//	m_PlayerQuestData[client_id] = std::map<unsigned int, unsigned int>();
				//if (m_PlayerQuestData[client_id].find(npc_index) == m_PlayerQuestData[client_id].end())
				//	m_PlayerQuestData[client_id][npc_index] = 0;
				//m_PlayerQuestData[client_id][npc_index] += 1;
				m_DbClient->setQuestAsActive(client_id, quest_id);
				msgStcSetQuestIp msg_data{ m_PlayersMsgs[client_id].get_ack_id(), quest_id };
				//msgStcSetQuestIp msg_data{ 3, 4 };
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_SET_QUEST_IP, msg_data);
				m_PlayersMsgs[client_id].add_new_msg(SocketHandler::m_Buffer, SocketHandler::m_Size, msg_data.ack_id);

				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
			}
			void ReceiveReward(Address sender)
			{
				auto [client_id, npc_index, dialog_index] = dsrl_types<IdType, unsigned int, unsigned int>(SocketHandler::m_Buffer + 1);
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
				SocketHandler::m_Size = construct_server_packet_with_buffer(SocketHandler::m_Buffer,
					MSG_STC_FINISH_QUEST, *data, size);
				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
			}
			void CharGotHit(Address sender)
			{
				auto [client_id, new_hp] = dsrl_types<IdType, unsigned int>(SocketHandler::m_Buffer + 1);
				m_DbClient->setValue(client_id, "hp", new_hp);
			}
			void CharUseSkillE(Address sender)
			{
				auto [player_id, skill_id, new_mp] = dsrl_types<IdType, unsigned int, unsigned int>(SocketHandler::m_Buffer + 1);
				m_DbClient->setValue(player_id, "mp", new_mp);
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_CHAR_USE_SKILL_E, player_id, skill_id);
				sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], SocketHandler::m_Size, player_id);
			}
			void CharUseSkillA(Address sender)
			{
				auto [player_id] = dsrl_types<IdType>(SocketHandler::m_Buffer + 1);
				const auto attack_skill_msg = dsrl_dynamic_type<AttackSkillMsg>(SocketHandler::m_Buffer + 5);
				CharAttackSkillMsg char_attack_msg{ player_id, attack_skill_msg };
				auto [data, size] = srl_dynamic_type(char_attack_msg);
				SocketHandler::m_Size = construct_server_packet_with_buffer(SocketHandler::m_Buffer, MSG_STC_CHAR_USE_SKILL_A, *data, size);
				sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], SocketHandler::m_Size, player_id);
				auto map_id = m_PlayerToMapId[player_id];
				auto& mob_manager = m_MapHolder[map_id].mob_manager;

				for (const auto& monster_hit : attack_skill_msg.monsters_hit)
					if (mob_manager.damageMob(monster_hit))
					{
						MobDied(map_id, player_id, monster_hit.monster_id);
					}
			}
			void MobDied(unsigned int map_id, unsigned int player_id, unsigned int monster_id)
			{
				UpdateQuestOnKill(player_id, monster_id);
				//auto dead_pos = m_MobManager.monsterDied(monster_id);
				auto& map_holder = m_MapHolder[map_id];
				auto dead_pos = m_MapHolder[map_id].mob_manager.monsterDied(monster_id);
				auto [data, size] = srl_dynamic_type(MonsterDiedMsg{ monster_id, {map_holder.addItem(0, 0, dead_pos)} });
				SocketHandler::m_Size = construct_server_packet_with_buffer(SocketHandler::m_Buffer, MSG_STC_MOB_DIED, *data, size);
				sendDataToAllClientsInMap(map_id, SocketHandler::m_Size);
				unsigned int exp = 30;
				if (m_PlayerToPartyLeaderMap.find(player_id) != m_PlayerToPartyLeaderMap.end())
				{
					auto amount_of_members = m_PartyLeaderToMembersMap[m_PlayerToPartyLeaderMap[player_id]].size();
					std::vector<std::tuple<double, double>> splits{ {0.85, 0.2}, {0.8, 0.15}, {0.75,0.14 }, {0.7, 0.13}, {0.65, 0.12} };
					auto [killer_s, party_s] = splits[amount_of_members - 2];
					unsigned int killer_split{ unsigned int(exp * killer_s) }, party_split{ unsigned int(exp * party_s) };
					for (const auto& party_member : m_PartyLeaderToMembersMap[m_PlayerToPartyLeaderMap[player_id]])
					{
						if (party_member == player_id)
						{
							SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_RECEIVED_EXP, killer_split);
							SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[party_member], SocketHandler::m_Buffer, SocketHandler::m_Size);
						}
						else
						{
							SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_RECEIVED_EXP, party_split);
							SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[party_member], SocketHandler::m_Buffer, SocketHandler::m_Size);
						}
					}
				}
				else
				{
					SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_RECEIVED_EXP, exp);
					SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[player_id], SocketHandler::m_Buffer, SocketHandler::m_Size);
				}
			}
			void PickItem(Address sender)
			{
				auto [player_id, item_id] = dsrl_types<IdType, unsigned int>(SocketHandler::m_Buffer + 1);
				auto player_map_id = m_PlayerToMapId[player_id];
				auto& items_dropped = m_MapHolder[player_map_id].items_dropped;
				auto item_picked = items_dropped[item_id];
				unsigned int item_loc = 0;
				if (item_picked.item_kind == network::USE_ITEM)
					item_loc = m_DbClient->addItem(item_picked.item_kind, player_id, item_picked.item_type_id);
				else if (item_picked.item_kind == network::EQUIP_ITEM)
					item_loc = m_DbClient->addEquipInv(player_id, item_picked.item_type_id);
				//std::cout << "inserted item " << item_picked.item_type_id << " at place " << item_loc << std::endl;
				items_dropped.erase(item_id);
				{
					SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_ADD_ITEM_TO_INVENTORY,
						item_picked.item_kind, item_loc, item_picked.item_type_id);
					SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
				}
				{
					SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_PICK_ITEM,
						PickItemMsg{ player_id, item_id });
					sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], SocketHandler::m_Size, player_id);
				}
				//sendDataToAllClients(m_Size);
			}
			void IncreaseSkill(Address sender)
			{
				auto [client_id, skill_id] = dsrl_types<IdType, unsigned int>(SocketHandler::m_Buffer + 1);
				m_DbClient->increaseSkillPoints(client_id, skill_id);
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_INCREASE_SKILL, skill_id);
				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
			}
			void WearEquip(Address sender)
			{
				auto [client_id, equip_type, equip_loc] = dsrl_types<IdType, SRL::EquipItemType, unsigned int>(SocketHandler::m_Buffer + 1);
				m_DbClient->wearEquip(client_id, equip_type, equip_loc);
			}
			void SwitchInventoryItems(Address sender)
			{
				auto [client_id, item_loc1, item_loc2, tab_index] =
					dsrl_types<IdType, unsigned int, unsigned int, unsigned int>(SocketHandler::m_Buffer + 1);
				if (tab_index == 0)
					m_DbClient->switchInventoryItems(client_id, item_loc1, item_loc2, tab_index);
				else if (tab_index == 1)
					m_DbClient->switchInventoryItems(client_id, item_loc1, item_loc2, tab_index);
			}
			void PlayerMsg(Address sender)
			{
				auto [player_id] =
					dsrl_types<IdType>(SocketHandler::m_Buffer + 1);
				auto msg = dsrl_dynamic_type<std::string>(SocketHandler::m_Buffer + 5);
				auto [data, size] = srl_dynamic_type(PlayerSayMsg{ player_id, msg });
				SocketHandler::m_Size = construct_server_packet_with_buffer(SocketHandler::m_Buffer, MSG_STC_PLAYER_SAY,
					*data, size);
				sendDataToAllClientsInMapExcept(m_PlayerToMapId[player_id], SocketHandler::m_Size, player_id);
			}
			void RequestParty(Address sender)
			{
				auto [client_id, player_to_invite] = dsrl_types<IdType, unsigned int>(SocketHandler::m_Buffer + 1);
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_PARTY_REQUEST, client_id);
				SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[player_to_invite], SocketHandler::m_Buffer, SocketHandler::m_Size);
			}
			void AcceptParty(Address sender)
			{
				auto [new_party_member, party_leader] = dsrl_types<unsigned int, unsigned int>(SocketHandler::m_Buffer + 1);
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
				SocketHandler::m_Size = construct_server_packet_with_buffer(SocketHandler::m_Buffer, MSG_STC_UPDATE_PARTY_STATE, *data, size);
				for (const auto& party_member : m_PartyLeaderToMembersMap[party_leader])
				{
					SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[party_member], SocketHandler::m_Buffer, SocketHandler::m_Size);
				}
			}
			void EnterPortal(Address sender)
			{
				auto [player_id, msg] = dsrl_types<IdType, EnterPortalMsg>(SocketHandler::m_Buffer + 1);
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_ENTER_PORTAL, player_id, msg);
				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);

				// change map
				auto prev_map = m_PlayerToMapId[player_id];
				m_MapHolder[msg.next_map].players_state[player_id] = m_MapHolder[prev_map].players_state[player_id];
				m_MapHolder[prev_map].players_state.erase(player_id);
				m_PlayerToMapId[player_id] = msg.next_map;
				m_MapHolder[msg.next_map].sendUpdateLocationToAll(sender);
				m_MapHolder[prev_map].sendPlayerLeftMap(player_id);
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
			void sendDropItem(unsigned int map_id, ItemDropMsg item_drop)
			{
				//m_ItemsDropped[item_drop.item_id] = item_drop;
				SocketHandler::m_Size = construct_server_packet(SocketHandler::m_Buffer, MSG_STC_DROP_ITEM, item_drop);

				sendDataToAllClientsInMap(map_id, SocketHandler::m_Size);
			}
			void sendDroppedItems(unsigned int map_id, Address sender)
			{
				std::vector<ItemDropMsg> dropped_items;
				//for (auto di : m_ItemsDropped)
				for (auto di : m_MapHolder[map_id].items_dropped)
					dropped_items.push_back(di.second);
				auto [data, size] = srl_dynamic_type(dropped_items);
				SocketHandler::m_Size = construct_server_packet_with_buffer(SocketHandler::m_Buffer,
					MSG_STC_DROPPED_ITEMS, *data, size);
				SocketHandler::m_Socket.Send(sender, SocketHandler::m_Buffer, SocketHandler::m_Size);
			}
			//void sendItemExpired(unsigned int item_id)
			//{
			//	m_ItemsDropped.erase(item_id);
			//	m_Size = create_client_packet_with_data(m_Buffer, MSG_STC_EXPIRE_ITEM, item_id);
			//	sendDataToAllClients(m_Size);
			//}
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
						SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[client_id], msg.msg_buffer, msg.buffer_size);
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
				m_PlayersState[client_id].npc_dialog_id[quest_id / 2]++;
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
					if (SocketHandler::m_ClientAddress[i].GetAddress() == address.GetAddress()
						&& SocketHandler::m_ClientAddress[i].GetPort() == address.GetPort())
						return i;
				}
				return -1;
			}
			void sendDataToAllClientsInMap(unsigned int map_id, unsigned int size)
			{
				//for (const auto& client : m_ClientAddress)
				//	if (client.GetAddress() != 0)
				//		m_Socket.Send(client, m_Buffer, size);
				for (const auto& [player_id, _ir] : m_MapHolder[map_id].players_state)
					SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[player_id], SocketHandler::m_Buffer, size);
			}
			//void sendDataToAllClients(unsigned int size)
			//{
			//	//for (const auto& client : m_ClientAddress)
			//	//	if (client.GetAddress() != 0)
			//	//		m_Socket.Send(client, m_Buffer, size);
			//	for (const auto& client_id : m_ClientsIds)
			//		m_Socket.Send(m_ClientAddress[client_id], m_Buffer, size);
			//}
			void sendDataToAllClientsInMapExcept(unsigned int map_id, unsigned int size, unsigned int exclude_id)
			{
				//const auto client_id_to_exclude = findExistingClientIndex(exclude_address);
				//for (const auto& client_id : m_ClientsIds)
				//	if (client_id != client_id_to_exclude)
				//		m_Socket.Send(m_ClientAddress[client_id], m_Buffer, size);
				//for (const auto& client_id : m_ClientsIds)
				for (const auto& [player_id, _ir] : m_MapHolder[map_id].players_state)
					if (player_id != exclude_id)
						SocketHandler::m_Socket.Send(SocketHandler::m_ClientAddress[player_id], SocketHandler::m_Buffer, size);
			}
			bool isClientConnected(int client_id) const
			{
				//return m_ClientConnected[clientIndex];
				return std::find(m_ClientsIds.begin(), m_ClientsIds.end(), client_id) != m_ClientsIds.end();
			}
			const Address& GetClientAddress(int clientIndex) const
			{
				return SocketHandler::m_ClientAddress[clientIndex];
			}
		};
	}
}