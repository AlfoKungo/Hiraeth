#pragma once
#include<cstdio>
//#include <windows.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
//#include "basic/a_timer.h"
#include "net/protocol.h"
#include "net/client_funcs.h"
#include "maths/vec2.h"
#include <map>
#include <cereal/archives/binary.hpp>
#include <iostream>
#include <fstream>
#include "game/net_chars/net_char_manager.h"
#include "game/monsters/monster_manager.h"
#include "net/srl_funcs.h"
#include "UI/ui_manager.h"
#include "basic/network_handler.h"
#include "net/net_msgs.h"
#include "NPC/npc_manager.h"
#include "map/map.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"  //ip address of udp server
const int BUFLEN = 512;  //Max length of buffer
const int PORT = 8888;   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {
		class ClientHandler : public NetworkHandler
		{
		private:
			const float KA_TIMEOUT = 1.0f, KA_LOSS_TIMEOUT = 3.0f;
			SOCKET m_Handle;
			unsigned int m_Id{ 0 };
			struct sockaddr_in m_SiOther {};
			int slen = sizeof(m_SiOther), m_SendSize;
			struct sockaddr_in m_sSiOther {};
			int sslen = sizeof(m_SiOther);
			//std::map<char, std::function<void()>> m_DistTable{};
			std::map<char, void(ClientHandler::*)()> m_DistTable{};
			//BufferType m_RcvBuffer[BUFLEN], m_SendBuffer[BUFLEN];
			char m_RcvBuffer[BUFLEN], m_SendBuffer[BUFLEN];
			ATimer m_KATimer{ KA_TIMEOUT }, m_KALossTimer{ KA_LOSS_TIMEOUT };
			PlayerData m_PlayerData;

			ui::UiManager* m_UiManager;
			game::NetCharManager* m_NetCharManager;
			game::MonsterManager* m_MonsterManager;
			item::ItemManager* m_ItemManager;
			skills::SkillManager* m_SkillManager;
			map::MapLayer* m_MapLayer;
			map::Map* m_Map;
			game::NpcManager* m_NpcManager;
			//std::map<unsigned int, maths::vec2> m_PlayerLocation;
			RegularMapUpdate m_PlayersLocationStruct;
			std::map<unsigned int, MonsterStateUpdate> m_Monsters;
		public:
			ClientHandler(ui::UiManager* ui_manager, game::NetCharManager* net_char_manager,
				game::MonsterManager* monster_manager, item::ItemManager* item_manager,
				skills::SkillManager* skill_manager, map::Map* map,
				game::NpcManager* npc_manager);

			~ClientHandler();
			void closeConnection();

			void Update(PlayerStateUpdateMsg psu);

		private:
			void bindFunctionToChar(char bytecode, void(ClientHandler::* fptr)())
			{
				//std::function<void()> val = std::bind(fptr, this);
				//std::function<void()> val = [&]() {(this->*fptr)(); };
				//m_DistTable.insert(std::pair<char, std::function<void()>>(bytecode, val));
				m_DistTable.insert(std::pair<char, void(ClientHandler::*)()>(bytecode, fptr));
			}
			void Send();
			//template<typename Ta>
			//void Send(unsigned char msg_id, Ta && msg)
			//{
			//	msg.player_id = m_Id;
			//	//m_SendSize = create_client_packet(m_SendBuffer, msg_id,msg);
			//	m_SendSize = create_client_packete(m_SendBuffer,msg);
			//	Send();
			//}
			template<typename T>
			void Send_dynamic(unsigned char msg_id, T&&  msg)
			{
				auto [data, size] = srl_dynamic_type(msg);
				m_SendSize = create_client_packet_with_buffer(m_SendBuffer, msg_id, m_Id, *data, size);
				Send();
			}
			template<typename ... Ts>
			void Send(unsigned char msg_id, Ts&& ... dts)
			{
				m_SendSize = create_client_packet(m_SendBuffer, msg_id, m_Id, dts ...);
				Send();
			}
			void Send(BufferType messgae_type, BufferType* data, int size);
			void sendStateUpdate(PlayerStateUpdateMsg char_state);

			void handleKeepAlive();

			void receiveData();

			//void addNewPlayerToMap(BufferType * buffer);
			void addNewPlayerToMap();
			void updateNetCharEquips();
			void updatePlayersLocation();
			void loadMobsData();
			void updateMobData();
			//void loadCurrentMapPlayers(BufferType * buffer);
			void loadCurrentMapPlayers();
			void recvMobHit();
			void recvMobDied();
			void recvStartDialog();
			void recvDialogNextSubDialog();
			void recvDialogSet();
			void recvPlayerUseSkillE(); //Effect
			void recvPlayerUseSkillA(); //Attack
			void recvPickItem();
			void recvDropItem();
			void recvDroppedItem();
			void recvExpireItem();
			void recvAddItem();
			void recvAddEquipItem();
			void recvIncreaseSkill();
			void recvPlayerSay();
			void recvSetQuestAsInProgress();
			void recvFinishQuest();
			void recvPartyRequest();
			void recvTradeRequest();
			void recvTradeFacilitte();
			void recvTradeAddItemToBox();
			void recvTradePlayerAccepted();
			void recvTradeFinalize ();
			void recvTradeCancel();
			void recvUpdatePartyState();
			void recvReceivedExp();
			void recvEnterPortal();
			void recvChangeMap();
			void change_map(unsigned int map_id);
			void recvPlayerLeft();
			void recvShopOpen();
		public:
			void sendAck(unsigned int ack_id);
			void sendAttackPacket(MonsterHit monster_damage) override;
			void sendNpcClick(unsigned int npc_id) override;
			void sendQuestProgress(unsigned int npc_id, unsigned int chat_id) override;
			//void sendQuestAccepted(unsigned int npc_id, unsigned int quest_id) override;
			void sendReceiveReward(unsigned int npc_id, unsigned int quest_id) override;
			void sendDialogButtonClicked(unsigned int npc_id, 
				unsigned int dialog_id, unsigned int sub_dialog_id, unsigned int button_clicked);
			void sendCharGotHit(unsigned int new_hp) override;
			void sendCharUseSkillE(unsigned int skill_id, unsigned int new_mp) override;
			void sendCharUseSkillA(unsigned int skill_id, std::vector<MonsterHit> monsters_hit) override;
			void sendPickItem(unsigned int item_id) override;
			void sendIncreaseSkill(unsigned int skill_id) override;
			void sendWearItem(SRL::EquipItemType item_type, unsigned int item_loc) override;
			void sendSwitchInventoryItems(unsigned int item_loc1, unsigned int item_loc2, unsigned int tab_index) override;
			void sendChatMsg(std::string) override;
			void sendEnterPortal(unsigned int portal_id) override;
			void sendRequestParty(unsigned int char_id) override;
			void sendRequestTrade(unsigned int char_id) override;
			void sendRequestInfo(unsigned int char_id) override;
			void sendDropItem(unsigned int item_id, unsigned int tab_index) override;
			void sendAddItemToTradeBox(unsigned int tab_index, unsigned int inventory_index, unsigned int trade_index) override;
			void sendAcceptTrade() override;
			void sendCancelTrade() override;
			void sendShopBuyItem(unsigned int item_number) override;
			PlayerData getPlayerData() const { return m_PlayerData; }
		};
	}
}
