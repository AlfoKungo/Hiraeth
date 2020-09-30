#include "hrth_pch.h"
#include "client_handler.h"
#include "basic/log.h"

namespace hiraeth::network {
	ClientHandler::ClientHandler(ui::UiManager* ui_manager, game::NetCharManager* net_char_manager,
		game::MonsterManager* monster_manager, item::ItemManager* item_manager,
		skills::SkillManager* skill_manager, map::Map* map,
		game::NpcManager* npc_manager)
		: m_RcvBuffer{ 0 }, m_SendBuffer{ 0 },
		m_UiManager{ ui_manager },
		m_NetCharManager{ net_char_manager },
		m_MonsterManager{ monster_manager },
		m_ItemManager{ item_manager },
		m_SkillManager{ skill_manager },
		m_Map{ map },
		m_MapLayer{ map->getMapLayer() },
		m_NpcManager{ npc_manager }
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("Failed. Error Code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		if ((m_Handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
		{
			printf("socket() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//setup address structure
		memset(reinterpret_cast<char*>(&m_SiOther), 0, sizeof(m_SiOther));
		m_SiOther.sin_family = AF_INET;
		m_SiOther.sin_port = htons(PORT);
		InetPton(AF_INET, _T(SERVER), &m_SiOther.sin_addr.S_un.S_addr);

		//const char message[] = "hello";
		//m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_OPEN_CONNECTION, m_Id, message, sizeof(message));
		//m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_OPEN_CONNECTION);
		auto [data, size] = srl_dynamic_type(std::string{ "ChrisS" });
		//auto[data, size] = srl_dynamic_type(std::string{"GusB"});
		m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_OPEN_CONNECTION, *data, size);
		Send();

		int recv_len;
		if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr*>(&m_SiOther), &slen)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != 10035)
				printf("recvfrom() failed with error code : %d", WSAGetLastError());
		}
		if (m_RcvBuffer[0] == MSG_STC_ESTABLISH_CONNECTION)
		{
			const auto msg = dsrl_dynamic_type<ConnectionEstablishMsg>(m_RcvBuffer + 1);
			m_PlayerData = msg.player_data;
			m_Id = msg.client_id;
			printf("received Id number of : %d\n", m_Id);
		}

		DWORD nonBlocking = 1;
		if (ioctlsocket(m_Handle, FIONBIO, &nonBlocking) != 0)
		{
			printf("failed to set non-blocking\n");
		}

		bindFunctionToChar(MSG_STC_ADD_PLAYER, &ClientHandler::addNewPlayerToMap);
		bindFunctionToChar(MSG_STC_UPDATE_NET_CHAR_EQUIPS, &ClientHandler::updateNetCharEquips);
		bindFunctionToChar(MSG_STC_PLAYERS_LOCATIONS, &ClientHandler::updatePlayersLocation);
		bindFunctionToChar(MSG_STC_PLAYERS_LIST, &ClientHandler::loadCurrentMapPlayers);
		bindFunctionToChar(MSG_STC_MOB_DATA, &ClientHandler::loadMobsData);
		bindFunctionToChar(MSG_STC_MOB_UPDATE, &ClientHandler::updateMobData);
		bindFunctionToChar(MSG_STC_MOB_HIT, &ClientHandler::recvMobHit);
		bindFunctionToChar(MSG_STC_MOB_DIED, &ClientHandler::recvMobDied);
		//bindFunctionToChar(MSG_STC_START_DIALOG, &ClientHandler::recvStartDialog);
		bindFunctionToChar(MSG_STC_NPC_START_DIALOG, &ClientHandler::recvStartDialog);
		bindFunctionToChar(MSG_STC_DIALOG_NEXT_SUB_DIALOG, &ClientHandler::recvDialogNextSubDialog);
		bindFunctionToChar(MSG_STC_DIALOG_SET, &ClientHandler::recvDialogSet);
		bindFunctionToChar(MSG_STC_CHAR_USE_SKILL_E, &ClientHandler::recvPlayerUseSkillE);
		bindFunctionToChar(MSG_STC_CHAR_USE_SKILL_A, &ClientHandler::recvPlayerUseSkillA);
		bindFunctionToChar(MSG_STC_PICK_ITEM, &ClientHandler::recvPickItem);
		bindFunctionToChar(MSG_STC_DROP_ITEM, &ClientHandler::recvDropItem);
		bindFunctionToChar(MSG_STC_DROPPED_ITEMS, &ClientHandler::recvDroppedItem);
		bindFunctionToChar(MSG_STC_EXPIRE_ITEM, &ClientHandler::recvExpireItem);
		bindFunctionToChar(MSG_STC_ADD_ITEM, &ClientHandler::recvAddItem);
		bindFunctionToChar(MSG_STC_ADD_EQUIP_ITEM, &ClientHandler::recvAddEquipItem);
		bindFunctionToChar(MSG_STC_INCREASE_SKILL, &ClientHandler::recvIncreaseSkill);
		bindFunctionToChar(MSG_STC_PLAYER_SAY, &ClientHandler::recvPlayerSay);
		bindFunctionToChar(MSG_STC_SET_QUEST_IP, &ClientHandler::recvSetQuestAsInProgress);
		bindFunctionToChar(MSG_STC_FINISH_QUEST, &ClientHandler::recvFinishQuest);
		bindFunctionToChar(MSG_STC_UPDATE_PARTY_STATE, &ClientHandler::recvUpdatePartyState);
		bindFunctionToChar(MSG_STC_RECEIVED_EXP, &ClientHandler::recvReceivedExp);
		bindFunctionToChar(MSG_STC_ENTER_PORTAL, &ClientHandler::recvEnterPortal);
		bindFunctionToChar(MSG_STC_CHANGE_MAP, &ClientHandler::recvChangeMap);
		bindFunctionToChar(MSG_STC_PARTY_REQUEST, &ClientHandler::recvPartyRequest);
		bindFunctionToChar(MSG_STC_TRADE_REQUEST, &ClientHandler::recvTradeRequest);
		bindFunctionToChar(MSG_STC_TRADE_FACILITATE, &ClientHandler::recvTradeFacilitte);
		bindFunctionToChar(MSG_STC_TRADE_ADD_ITEM_TO_BOX, &ClientHandler::recvTradeAddItemToBox);
		bindFunctionToChar(MSG_STC_TRADE_PLAYER_ACCEPTED, &ClientHandler::recvTradePlayerAccepted);
		bindFunctionToChar(MSG_STC_TRADE_FINALIZE, &ClientHandler::recvTradeFinalize);
		bindFunctionToChar(MSG_STC_TRADE_CANCEL, &ClientHandler::recvTradeCancel);
		bindFunctionToChar(MSG_STC_PLAYER_LEFT, &ClientHandler::recvPlayerLeft);
		bindFunctionToChar(MSG_STC_SHOP_OPEN, &ClientHandler::recvShopOpen);

		//EventManager *m_EventManager = EventManager::Instance();
		//m_EventManager->createEvent<unsigned int>(SendIncreaseSkill);
		//m_EventManager->subscribe(SendIncreaseSkill, this, &ClientHandler::sendIncreaseSkill);
		//m_EventManager->subscribe(ItemWore, this, &ClientHandler::sendItemWore);
		//m_EventManager->subscribe(SwitchInventoryItems, this, &ClientHandler::sendSwitchInventoryItems);
	}

	ClientHandler::~ClientHandler()
	{
		closeConnection();
	}

	void ClientHandler::closeConnection()
	{
		char message[] = "hello";
		Send(MSG_CTS_CLOSE_CONNECTION, message, sizeof(message));
		//m_SendSize = construct_client_packet(m_SendBuffer, MSG_CTS_CLOSE_CONNECTION, m_Id, message, sizeof(message));
		//if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
		//{
		//	printf("sendto() failed with error code : %d\n", WSAGetLastError());
		//	exit(EXIT_FAILURE);
		//}
		//printf("Sent keep_alive\n");
	}

	void ClientHandler::Update(PlayerStateUpdateMsg psu)
	{
		handleKeepAlive();
		//sendLocationUpdate(char_pos);
		sendStateUpdate(psu);
		receiveData();
	}

	void ClientHandler::Send()
	{
		if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr*>(&m_SiOther), slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}

	void ClientHandler::Send(BufferType messgae_type, BufferType* data, int size)
	{
		m_SendSize = create_client_packet_with_buffer(m_SendBuffer, messgae_type, m_Id, data, size);
		Send();
		//if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
		//{
		//	printf("sendto() failed with error code : %d\n", WSAGetLastError());
		//	exit(EXIT_FAILURE);
		//}
	}

	void ClientHandler::sendStateUpdate(PlayerStateUpdateMsg char_state)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_LOCATION_UPDATE, m_Id, char_state);
		//Send();
		Send(MSG_CTS_LOCATION_UPDATE, char_state);
	}

	void ClientHandler::handleKeepAlive()
	{
		if (m_KATimer.hasExpired())
		{
			const char message[] = "hello";
			m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_KA, m_Id, message, sizeof(message));
			if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr*>(&m_SiOther), slen) == SOCKET_ERROR)
			{
				printf("sendto() failed with error code : %d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			//printf("Sent keep_alive\n");
			m_KATimer.reSet(KA_TIMEOUT);
		}
		memset(m_RcvBuffer, '\0', BUFLEN);
		if (m_KALossTimer.hasExpired())
		{
			printf("I lost keep_alive\n");
			m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
		}
	}

	void ClientHandler::receiveData()
	{
		while (true)
		{
			int recv_len;

			if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr*>(&m_SiOther), &slen)) == SOCKET_ERROR)
				//if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&m_sSiOther), &sslen)) == SOCKET_ERROR)
				//if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&si_other), &sslen)) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != 10035)
				{
					printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
					std::exit(EXIT_SUCCESS);
				}
			}
			if (recv_len > 0)
			{
				if (m_DistTable.find(m_RcvBuffer[0]) != m_DistTable.end())
				{
					(this->*m_DistTable[m_RcvBuffer[0]])();
				}
			}
			else
				return;
		}
	}

	//void ClientHandler::addNewPlayerToMap(BufferType * buffer)
	void ClientHandler::addNewPlayerToMap()
	{
		//const auto new_char_id = dsrl_type<unsigned int>(buffer + 1);
		const auto new_player_data = dsrl_dynamic_type<MsgStcNewPlayerInMap>(m_RcvBuffer + 1);
		m_NetCharManager->addChar(new_player_data.player_id, maths::vec2{ 0,0 });
		for (auto equip : new_player_data.player_equips)
		{
			m_NetCharManager->getCharsMap()[new_player_data.player_id]->wearItem(equip.second);
		}
	}

	void ClientHandler::updateNetCharEquips()
	{
		const auto new_player_data = dsrl_dynamic_type<MsgStcUpdateNetCharEquips>(m_RcvBuffer + 1);
		m_NetCharManager->getCharsMap()[new_player_data.player_id]->clearEquips();
		for (auto equip : new_player_data.player_equips)
		{
			m_NetCharManager->getCharsMap()[new_player_data.player_id]->wearItem(equip.second);
		}
	}

	void ClientHandler::updatePlayersLocation()
	{
		dsrl_dynamic_type(m_PlayersLocationStruct, m_RcvBuffer + 1);
		for (const auto& [index, player_state] : m_PlayersLocationStruct.m_PlayersLocation)
			m_NetCharManager->updateCharsState(index, player_state);
		m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
	}

	void ClientHandler::loadMobsData()
	{
		dsrl_dynamic_type(m_Monsters, m_RcvBuffer + 1);
		for (const auto& [index, monster] : m_Monsters)
			m_MonsterManager->addMonster(index, monster);
	}

	void ClientHandler::updateMobData()
	{
		auto [monster_index, monster_state] =
			dsrl_type<MonsterStateUpdateMsg>(m_RcvBuffer + 1);
		m_MonsterManager->updateMonster(monster_index, monster_state);
	}

	//void ClientHandler::loadCurrentMapPlayers(BufferType * buffer)
	void ClientHandler::loadCurrentMapPlayers()
	{
		//dsrl_dt_packet_data(m_PlayerLocation, m_RcvBuffer + 1);
		//m_PlayerLocation.erase(m_Id);
		//for (const auto& player : m_PlayerLocation)
		//	m_NetCharManager->updateCharPos(player.first, player.second);
		//m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
	}

	void ClientHandler::recvMobHit()
	{
		//auto monster_damage = dsrl_type<MonsterDamage>(m_RcvBuffer + 1);
		auto monster_damage = dsrl_type<MonsterHit>(m_RcvBuffer + 1);
		auto monster = m_MonsterManager->getMonsters()->at(monster_damage.monster_id);
		monster->getHit(static_cast<game::Direction>(monster_damage.dir),
			game::Damage{ unsigned int(monster_damage.damage), 0 });
	}

	void ClientHandler::recvMobDied()
	{
		auto monster_died_msg = dsrl_dynamic_type<MonsterDiedMsg>(m_RcvBuffer + 1);
		//m_MonsterManager->killMonster(monster_died_msg.monster_id);
		float x = 0.75f - float(monster_died_msg.dropped_items.size()) * (0.75f);
		for (const auto& item : monster_died_msg.dropped_items)
		{
			m_ItemManager->dropItem(item.item_id, item.item_type_id,
				item.item_kind, item.location, x);
			x += 1.5f;
		}
	}

	void ClientHandler::recvStartDialog()
	{
		const auto [npc_id, dialog_id] = dsrl_types<unsigned int, unsigned int>(m_RcvBuffer + 1);
		EventManager* m_EventManager = EventManager::Instance();
		m_EventManager->execute<unsigned int>(DialogStart, npc_id, dialog_id);
	}

	void ClientHandler::recvDialogNextSubDialog()
	{
		//const auto msg = dsrl_types<MsgStcDialogNextSubDialog>(m_RcvBuffer + 1);
		auto dialog_manager = m_NpcManager->getDialogManager();
		dialog_manager->nextSubDialog();
	}

	void ClientHandler::recvDialogSet()
	{
		const auto msg = dsrl_type<MsgStcDialogSet>(m_RcvBuffer + 1);
		auto dialog_manager = m_NpcManager->getDialogManager();
		dialog_manager->startDialog(msg.npc_id, msg.dialog_id);
	}

	void ClientHandler::recvPlayerUseSkillE()
	{
		const auto [client_id, skill_id] = dsrl_types<unsigned int, unsigned int>(m_RcvBuffer + 1);
		m_NetCharManager->charUseSkillE(client_id, skill_id);
	}

	void ClientHandler::recvPlayerUseSkillA()
	{
		const auto CharAttackMsg = dsrl_dynamic_type<CharAttackSkillMsg>(m_RcvBuffer + 1);
		m_NetCharManager->charUseSkillA(CharAttackMsg.char_id, CharAttackMsg.attack_msg);
	}

	void ClientHandler::recvPickItem()
	{
		const auto pick_item_msg = dsrl_type<PickItemMsg>(m_RcvBuffer + 1);
		//m_NetCharManager->charPickItem(pick_item_msg.char_id, pick_item_msg.item_id);
		auto* item = m_ItemManager->getItem(pick_item_msg.item_id);
		item->pickUp(&m_NetCharManager->getCharsMap()[pick_item_msg.char_id]->getBounds());
	}

	void ClientHandler::recvDropItem()
	{
		const auto item_drop_msg = dsrl_type<MsgStcDropItem>(m_RcvBuffer + 1);
		m_ItemManager->dropItem(item_drop_msg.item_id, item_drop_msg.item_type_id,
			item_drop_msg.item_kind, item_drop_msg.pos, 0.0f);
	}

	void ClientHandler::recvDroppedItem()
	{
		//const auto item_drop_msg = dsrl_type<ItemDropMsg>(m_RcvBuffer + 1);
		const auto dropped_items = dsrl_dynamic_type<std::vector<ItemDropData>>(m_RcvBuffer + 1);
		for (const auto& item : dropped_items)
			m_ItemManager->dropItem(item.item_id, item.item_type_id,
				item.item_kind, item.location, 0.0f);
	}

	void ClientHandler::recvExpireItem()
	{
		const auto expired_item_id = dsrl_type<unsigned int>(m_RcvBuffer + 1);
		m_ItemManager->startExpiring(expired_item_id);
		//for (const auto& item : dropped_items)
		//	m_ItemManager->dropItem(item.item_id, item.item_type_id, item.item_kind, item.location);
	}

	void ClientHandler::recvAddItem()
	{
		const auto [item_kind, item_loc, item_id] = dsrl_type<AddItemMsg>(m_RcvBuffer + 1);
		m_ItemManager->addItemToInv(item_kind, item_loc, item_id);
	}

	void ClientHandler::recvAddEquipItem()
	{
		const auto [item_loc, item_id, item_info] = dsrl_dynamic_type<AddEquipItemMsg>(m_RcvBuffer + 1);
		m_ItemManager->addEquipItemToInv(item_info, item_loc, item_id);
	}

	void ClientHandler::recvIncreaseSkill()
	{
		const auto increased_skill_id = dsrl_type<unsigned int>(m_RcvBuffer + 1);
		m_SkillManager->increasSkill(increased_skill_id);
	}

	void ClientHandler::recvPlayerSay()
	{
		const auto msg = dsrl_dynamic_type<PlayerSayMsg>(m_RcvBuffer + 1);
		m_NetCharManager->getCharsMap()[msg.char_id]->writeSayBubble(msg.say_msg);
	}

	void ClientHandler::recvSetQuestAsInProgress()
	{
		const auto msg_data = dsrl_type<msgStcSetQuestIp>(m_RcvBuffer + 1);
		m_UiManager->getUiQuests()->setQuestAsActive(msg_data.quest_id);
		sendAck(msg_data.ack_id);
	}

	void ClientHandler::recvFinishQuest()
	{
		const auto fq_msg = dsrl_dynamic_type<FinishQuestMsg>(m_RcvBuffer + 1);
		m_UiManager->getUiQuests()->setQuestAsDone(fq_msg.quest_id);
	}

	void ClientHandler::recvPartyRequest()
	{
		const auto char_requesting = dsrl_type<unsigned int>(m_RcvBuffer + 1);
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_ACCEPT_PARTY, m_Id, char_requesting);
		//Send();
		Send(MSG_CTS_ACCEPT_PARTY, char_requesting);
	}

	void ClientHandler::recvTradeRequest()
	{
		const auto msg = dsrl_type<MsgStcTradeRequest>(m_RcvBuffer + 1);
		Send(MSG_CTS_TRADE_REQUEST_ACCEPT, MsgCtsTradeRequestAccept{ msg.player_who_wants_to_trade });
	}

	void ClientHandler::recvTradeFacilitte()
	{
		const auto msg = dsrl_type<MsgStcTradeFacilitate>(m_RcvBuffer + 1);
		m_UiManager->getUiTrade()->openTrade(CharManager::Instance()->getEquips(), m_NetCharManager->getCharsMap()[msg.player_to_trade_with]);
	}
	void ClientHandler::recvTradeAddItemToBox()
	{
		const auto msg = dsrl_dynamic_type<MsgStcTradeAddItemToBox>(m_RcvBuffer + 1);
		m_UiManager->getUiTrade()->addItem(msg.trade_index, msg.equip_info, msg.is_me);
	}

	void ClientHandler::recvTradePlayerAccepted()
	{
		const auto msg = dsrl_type<MsgStcTradePlayerAccepted>(m_RcvBuffer + 1);
		m_UiManager->getUiTrade()->playerAccepted(msg.is_me);
	}

	void ClientHandler::recvTradeFinalize()
	{
		m_UiManager->getUiTrade()->closeTrade();
	}

	void ClientHandler::recvTradeCancel()
	{
		m_UiManager->getUiTrade()->closeTrade();
	}

	void ClientHandler::recvUpdatePartyState()
	{
		const auto msg = dsrl_dynamic_type<PartyUpdateMsg>(m_RcvBuffer + 1);
		auto ui_party = m_UiManager->getUiParty();
		ui_party->set_party(msg.party_members);
	}

	void ClientHandler::recvReceivedExp()
	{
		const auto exp_amount = dsrl_type<unsigned int>(m_RcvBuffer + 1);
		m_UiManager->getMainUi()->getCharacterStats()->increaseExp(exp_amount);
	}

	void ClientHandler::recvEnterPortal()
	{
		const auto msg = dsrl_type<EnterPortalMsg>(m_RcvBuffer + 1);
		change_map(msg.next_map);
	}

	void ClientHandler::recvChangeMap()
	{
		const auto msg = dsrl_type<ChangeMapMsg>(m_RcvBuffer + 1);
		change_map(msg.next_map);
	}

	void ClientHandler::change_map(unsigned int map_id)
	{
		m_NetCharManager->clearChars();
		m_MonsterManager->clearMonsters();
		m_ItemManager->clearItems();
		m_Map->change_map(map_id);
		m_NpcManager->clearNpcs();
		m_NpcManager->loadNpcs();
	}

	void ClientHandler::recvPlayerLeft()
	{
		const auto msg = dsrl_type<PlayerLeftMsg>(m_RcvBuffer + 1);
		m_NetCharManager->deleteChar(msg.player_left_id);
	}

	void ClientHandler::recvShopOpen()
	{
		const auto msg = dsrl_dynamic_type<MsgStcShopOpen>(m_RcvBuffer + 1);
		m_UiManager->getUiShop()->openShop(msg.sell_items);
	}

	void ClientHandler::sendAck(unsigned int ack_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_ACK, m_Id, ack_id);
		//Send();
		Send(MSG_CTS_ACK, ack_id);
	}

	void ClientHandler::sendAttackPacket(MonsterHit monster_damage)
		//void ClientHandler::sendAttackPacket(MonsterDamage monster_damage)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_HIT_MOB, m_Id, monster_damage);
		//Send();
		Send(MSG_CTS_HIT_MOB, monster_damage);
		//Log::Init();
		//HH_LOG("myfirst_log");
		//HH_LOG("mysncnd_log");
	}

	void ClientHandler::sendNpcClick(unsigned int npc_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_NPC_CLICK, m_Id, npc_id);
		//Send();
		Send(MSG_CTS_NPC_CLICK, npc_id);
	}

	void ClientHandler::sendQuestProgress(unsigned int npc_id, unsigned int chat_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_DIALOG_NEXT, m_Id, npc_id, chat_id);
		//Send();
		Send(MSG_CTS_DIALOG_NEXT, npc_id, chat_id);
	}

	//void ClientHandler::sendQuestAccepted(unsigned int npc_id, unsigned int quest_id)
	//{
	//	Send(MSG_CTS_ACCEPT_QUEST, npc_id, quest_id);
	//}

	void ClientHandler::sendReceiveReward(unsigned npc_id, unsigned quest_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_FINISH_QUEST, m_Id, npc_id, quest_id);
		//Send();
		Send(MSG_CTS_FINISH_QUEST, npc_id, quest_id);
	}

	//#define NAME(name, ...) name##_ID, name{__VA_ARGS__}
	void ClientHandler::sendDialogButtonClicked(unsigned int npc_id, unsigned int dialog_id, unsigned int sub_dialog_id, unsigned int button_clicked)
	{
		Send(MSG_CTS_DIALOG_BUTTON_CLICKED, MsgCtsDialogButtonClick{ npc_id, dialog_id, sub_dialog_id, button_clicked });
		//Send(NAME(MsgCtsDialogButtonClick, npc_id, quest_id, button_clicked ));
	}

	void ClientHandler::sendCharGotHit(unsigned int new_hp)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_CHAR_GOT_HIT, m_Id, new_hp);
		//Send();
		Send(MSG_CTS_CHAR_GOT_HIT, new_hp);
	}

	void ClientHandler::sendCharUseSkillE(unsigned int skill_id, unsigned int new_mp)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_CHAR_USE_SKILL_E, m_Id, skill_id, new_mp);
		//Send();
		Send(MSG_CTS_CHAR_USE_SKILL_E, skill_id, new_mp);
	}

	void ClientHandler::sendCharUseSkillA(unsigned int skill_id, std::vector<MonsterHit> monsters_hit)
	{
		AttackSkillMsg msg{ skill_id, monsters_hit };
		//auto [data, size] = srl_dynamic_type(msg);
		//m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_CHAR_USE_SKILL_A, m_Id, *data, size);
		//Send();
		Send_dynamic(MSG_CTS_CHAR_USE_SKILL_A, msg);
	}

	void ClientHandler::sendPickItem(unsigned int item_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_PICK_ITEM, m_Id, item_id);
		//Send();
		Send(MSG_CTS_PICK_ITEM, item_id);
	}

	void ClientHandler::sendIncreaseSkill(unsigned skill_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_INCREASE_SKILL, m_Id, skill_id);
		//Send();
		Send(MSG_CTS_INCREASE_SKILL, skill_id);
	}

	void ClientHandler::sendWearItem(SRL::EquipItemType item_type, unsigned int item_loc)
	{
		//auto equip = m_UiManager->getUiEquip()->getEquip(item_type);

		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_WEAR_EQUIP, m_Id, item_type, item_loc);
		//Send();
		Send(MSG_CTS_WEAR_EQUIP, item_type, item_loc);
	}

	void ClientHandler::sendSwitchInventoryItems(unsigned item_loc1, unsigned item_loc2, unsigned tab_index)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_INVENTORY_ACTION, m_Id,
		//	item_loc1, item_loc2, tab_index);
		//Send();
		Send(MSG_CTS_INVENTORY_ACTION,
			item_loc1, item_loc2, tab_index);
	}

	void ClientHandler::sendChatMsg(std::string msg)
	{
		//auto [data, size] = srl_dynamic_type(msg);
		//m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_PLAYER_SAY, m_Id, *data, size);
		//Send();
		Send_dynamic(MSG_CTS_PLAYER_SAY, msg);
	}
	void ClientHandler::sendEnterPortal(unsigned int portal_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_ENTER_PORTAL, m_Id, EnterPortalMsg{ portal_id });
		//Send();
		Send(MSG_CTS_ENTER_PORTAL, EnterPortalMsg{ portal_id });
	}
	void ClientHandler::sendRequestParty(unsigned int char_id)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_PARTY_REQUEST, m_Id, char_id);
		//Send();
		Send(MSG_CTS_PARTY_REQUEST, char_id);
	}
	void ClientHandler::sendRequestTrade(unsigned int char_id)
	{
		Send(MSG_CTS_TRADE_REQUEST, MsgCtsTradeRequest{ char_id });
	}
	void ClientHandler::sendRequestInfo(unsigned int char_id)
	{
	}
	void ClientHandler::sendDropItem(unsigned int item_id, unsigned int tab_index)
	{
		//m_SendSize = create_client_packet(m_SendBuffer, MSG_CTS_DROP_ITEM,
		//	m_Id, item_id, tab_index);
		//Send();
		Send(MSG_CTS_DROP_ITEM, item_id, tab_index);
	}
	void ClientHandler::sendAddItemToTradeBox(unsigned int tab_index, unsigned int inventory_index, unsigned int trade_index)
	{
		Send(MSG_CTS_TRADE_ADD_ITEM_TO_BOX, MsgCtsTradeAddItemToBox{ tab_index, inventory_index, trade_index });
	}
	void ClientHandler::sendAcceptTrade()
	{
		Send(MSG_CTS_TRADE_ACCEPT);
	}
	void ClientHandler::sendCancelTrade()
	{
		Send(MSG_CTS_TRADE_CANCEL);
	}
	void ClientHandler::sendShopBuyItem(unsigned int item_number)
	{
		Send(MSG_STC_SHOP_BUY_ITEM, MsgCtsShopBuyItem{ item_number });
	}
}