#include "client_handler.h"
#include "basic/log.h"

namespace hiraeth {
	namespace network {

		ClientHandler::ClientHandler(game::NetCharManager * net_char_manager, game::MonsterManager * monster_manager,
			item::ItemManager * item_manager)
			: m_RcvBuffer{ 0 }, m_SendBuffer{ 0 },
			m_NetCharManager(net_char_manager),
			m_MonsterManager{ monster_manager },
			m_ItemManager(item_manager)
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
			memset(reinterpret_cast<char *>(&m_SiOther), 0, sizeof(m_SiOther));
			m_SiOther.sin_family = AF_INET;
			m_SiOther.sin_port = htons(PORT);
			InetPton(AF_INET, _T(SERVER), &m_SiOther.sin_addr.S_un.S_addr);

			//const char message[] = "hello";
			//m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_OPEN_CONNECTION, m_Id, message, sizeof(message));
			//m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_OPEN_CONNECTION);
			auto[data, size] = srl_dynamic_type(std::string{"ChrisS"});
			//auto[data, size] = srl_dynamic_type(std::string{"GusB"});
			m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_OPEN_CONNECTION, *data, size);
			Send();

			int recv_len;
			if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), &slen)) == SOCKET_ERROR)
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
			bindFunctionToChar(MSG_STC_PLAYERS_LOCATIONS, &ClientHandler::updatePlayersLocation);
			bindFunctionToChar(MSG_STC_PLAYERS_LIST, &ClientHandler::loadCurrentMapPlayers);
			bindFunctionToChar(MSG_STC_MOB_DATA, &ClientHandler::loadMobsData);
			bindFunctionToChar(MSG_STC_MOB_UPDATE, &ClientHandler::updateMobData);
			bindFunctionToChar(MSG_STC_MOB_HIT, &ClientHandler::recvMobHit);
			bindFunctionToChar(MSG_STC_MOB_DIED, &ClientHandler::recvMobDied);
			bindFunctionToChar(MSG_STC_START_DIALOG, &ClientHandler::recvStartDialog);
			bindFunctionToChar(MSG_STC_CHAR_USE_SKILL_E, &ClientHandler::recvPlayerUseSkillE);
			bindFunctionToChar(MSG_STC_CHAR_USE_SKILL_A, &ClientHandler::recvPlayerUseSkillA);
			bindFunctionToChar(MSG_STC_PICK_ITEM, &ClientHandler::recvPickItem);
			bindFunctionToChar(MSG_STC_DROP_ITEM, &ClientHandler::recvDropItem);
			bindFunctionToChar(MSG_STC_DROPPED_ITEM, &ClientHandler::recvDroppedItem);
			bindFunctionToChar(MSG_STC_EXPIRE_ITEM, &ClientHandler::recvExpireItem);
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
			printf("Sent keep_alive\n");
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
			if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
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
			char message[sizeof(char_state)];
			memcpy(message, &char_state, sizeof(char_state));
			Send(MSG_CTS_LOCATION_UPDATE, message, sizeof(message));
			//m_SendSize = construct_client_packet(m_SendBuffer, MSG_CTS_LOCATION_UPDATE, m_Id, message, sizeof(message));
			//if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
			//{
			//	printf("sendto() failed with error code : %d\n", WSAGetLastError());
			//	exit(EXIT_FAILURE);
			//}
		}

		void ClientHandler::handleKeepAlive()
		{
			if (m_KATimer.hasExpired())
			{
				const char message[] = "hello";
				m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_KA, m_Id, message, sizeof(message));
				if (sendto(m_Handle, m_SendBuffer, m_SendSize, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d\n", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				printf("Sent keep_alive\n");
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
				if ((recv_len = recvfrom(m_Handle, m_RcvBuffer, BUFLEN, 0, reinterpret_cast<struct sockaddr *>(&m_SiOther), &slen)) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != 10035)
					{
						std::exit(EXIT_SUCCESS);
						printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
					}
				}
				if (recv_len > 0)
				{
					if (m_DistTable.find(m_RcvBuffer[0]) != m_DistTable.end())
					{
						(this->*m_DistTable[m_RcvBuffer[0]])();
					}
					//switch (m_RcvBuffer[0])
					//{
					//case MSG_STC_ADD_PLAYER:
					//	addNewPlayerToMap();
					//	break;
					//case MSG_STC_PLAYERS_LOCATIONS:
					//	updatePlayersLocation();
					//	break;
					//case MSG_STC_PLAYERS_LIST:
					//	loadCurrentMapPlayers(m_RcvBuffer);
					//	break;
					//case MSG_STC_MOB_DATA:
					//	loadMobsData();
					//	break;
					//case MSG_STC_MOB_UPDATE:
					//	updateMobData();
					//	break;
					//case MSG_STC_MOB_HIT:
					//	recvMobHit();
					//	break;
					//case MSG_STC_MOB_DIED:
					//	recvMobDied();
					//	break;
					//case MSG_STC_START_DIALOG:
					//	recvStartDialog();
					//	break;
					//case MSG_STC_CHAR_USE_SKILL_E:
					//	recvPlayerUseSkillE();
					//	break;
					//case MSG_STC_CHAR_USE_SKILL_A:
					//	recvPlayerUseSkillA();
					//	break;
					//case MSG_STC_PICK_ITEM:
					//	recvPickItem();
					//	break;
					//case MSG_STC_DROP_ITEM:
					//	recvDropItem();
					//	break;
					//case MSG_STC_DROPPED_ITEM:
					//	recvDroppedItem();
					//	break;
					//case MSG_STC_EXPIRE_ITEM:
					//	recvExpireItem();
					//	break;
					//default:
					//	break;
					//}
				}
				else
					return;
			}
		}

		//void ClientHandler::addNewPlayerToMap(BufferType * buffer)
		void ClientHandler::addNewPlayerToMap()
		{
			//const auto new_char_id = dsrl_type<unsigned int>(buffer + 1);
			const auto new_char_id = dsrl_type<unsigned int>(m_RcvBuffer + 1);
			m_NetCharManager->addChar(new_char_id, maths::vec2{ 0,0 });
		}

		void ClientHandler::updatePlayersLocation()
		{
			dsrl_dynamic_type(m_PlayersLocationStruct, m_RcvBuffer + 1);
			for (const auto&[index, player_state] : m_PlayersLocationStruct.m_PlayersLocation)
				m_NetCharManager->updateCharsState(index, player_state);
			m_KALossTimer.reSet(KA_LOSS_TIMEOUT);
		}

		void ClientHandler::loadMobsData()
		{
			dsrl_dynamic_type(m_Monsters, m_RcvBuffer + 1);
			for (const auto&[index, monster] : m_Monsters)
				m_MonsterManager->addMonster(index, monster);
		}

		void ClientHandler::updateMobData()
		{
			auto[monster_index, monster_state] =
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
			//auto dead_monster_id = dsrl_types<unsigned int>(m_RcvBuffer + 1);
			auto monster_died_msg = dsrl_dynamic_type<MonsterDiedMsg>(m_RcvBuffer + 1);
			//m_MonsterManager->killMonster(monster_died_msg.monster_id, monster_died_msg.dropped_items);
			m_MonsterManager->killMonster(monster_died_msg.monster_id);
			for (const auto& item : monster_died_msg.dropped_items)
				m_ItemManager->dropItem(item.item_id, item.item_type_id, item.item_kind, item.location);
		}

		void ClientHandler::recvStartDialog()
		{
			const auto dialog_id = dsrl_type<unsigned int>(m_RcvBuffer + 1);
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->execute<unsigned int>(DialogStart, dialog_id);
		}

		void ClientHandler::recvPlayerUseSkillE()
		{
			const auto[client_id, skill_id] = dsrl_types<unsigned int, unsigned int>(m_RcvBuffer + 1);
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
			//m_NetCharManager->charUseSkillA(CharAttackMsg.char_id, CharAttackMsg.attack_msg);
			m_NetCharManager->charPickItem(pick_item_msg.char_id, pick_item_msg.item_id);
		}

		void ClientHandler::recvDropItem()
		{
			const auto item_drop_msg = dsrl_type<ItemDropMsg>(m_RcvBuffer + 1);
			m_ItemManager->dropItem(item_drop_msg.item_id, item_drop_msg.item_type_id, item_drop_msg.item_kind, item_drop_msg.location);
		}

		void ClientHandler::recvDroppedItem()
		{
			//const auto item_drop_msg = dsrl_type<ItemDropMsg>(m_RcvBuffer + 1);
			const auto dropped_items = dsrl_dynamic_type<std::vector<ItemDropMsg>>(m_RcvBuffer + 1);
			for (const auto& item : dropped_items)
				m_ItemManager->dropItem(item.item_id, item.item_type_id, item.item_kind, item.location);
		}

		void ClientHandler::recvExpireItem()
		{
			const auto expired_item_id = dsrl_type<unsigned int>(m_RcvBuffer + 1);
			m_ItemManager->expireItem(expired_item_id);
			//for (const auto& item : dropped_items)
			//	m_ItemManager->dropItem(item.item_id, item.item_type_id, item.item_kind, item.location);
		}

		void ClientHandler::sendAttackPacket(MonsterHit monster_damage)
		//void ClientHandler::sendAttackPacket(MonsterDamage monster_damage)
		{
			m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_HIT_MOB, m_Id, monster_damage);
			Send();
			//Log::Init();
			//HH_LOG("myfirst_log");
			//HH_LOG("mysncnd_log");
		}

		void ClientHandler::sendNpcClick(unsigned int npc_id)
		{
			m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_NPC_CLICK, m_Id, npc_id);
			Send();
		}

		void ClientHandler::sendQuestProgress(unsigned int npc_id, unsigned int chat_id)
		{
			m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_DIALOG_NEXT, m_Id, npc_id, chat_id);
			Send();
		}

		void ClientHandler::sendQuestAccepted(unsigned int npc_id, unsigned int quest_id)
		{
			m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_ACCEPT_QUEST, m_Id, npc_id, quest_id);
			Send();
		}

		void ClientHandler::sendCharGotHit(unsigned int new_hp)
		{
			m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_CHAR_GOT_HIT, m_Id, new_hp);
			Send();
		}

		void ClientHandler::sendCharUseSkillE(unsigned int skill_id, unsigned int new_mp)
		{
			m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_CHAR_USE_SKILL_E, m_Id, skill_id, new_mp);
			Send();
		}

		void ClientHandler::sendCharUseSkillA(unsigned int skill_id, std::vector<MonsterHit> monsters_hit)
		{
			AttackSkillMsg msg{ skill_id, monsters_hit };
			auto[data, size] = srl_dynamic_type(msg);
			m_SendSize = create_client_packet_with_buffer(m_SendBuffer, MSG_CTS_CHAR_USE_SKILL_A, m_Id, *data, size);
			Send();
		}

		void ClientHandler::sendPickItem(unsigned int item_id)
		{
			//AttackSkillMsg msg{ skill_id, monsters_hit };
			//auto[data, size] = srl_dynamic_type(msg);
			m_SendSize = create_client_packet_with_data(m_SendBuffer, MSG_CTS_PICK_ITEM, m_Id, item_id);
			Send();
		}
	}
}
