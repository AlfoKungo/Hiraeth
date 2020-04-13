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
			unsigned int m_Id{0};
			struct sockaddr_in m_SiOther {};
			//std::map<char, std::function<void()>> m_DistTable{};
			std::map<char, void(ClientHandler::*)()> m_DistTable{};
			int slen = sizeof(m_SiOther), m_SendSize;
			//BufferType m_RcvBuffer[BUFLEN], m_SendBuffer[BUFLEN];
			char m_RcvBuffer[BUFLEN], m_SendBuffer[BUFLEN];
			ATimer m_KATimer{ KA_TIMEOUT }, m_KALossTimer{ KA_LOSS_TIMEOUT };
			PlayerData m_PlayerData;

			ui::UiManager * m_UiManager;
			game::NetCharManager * m_NetCharManager;
			game::MonsterManager * m_MonsterManager;
			item::ItemManager * m_ItemManager;
			skills::SkillManager * m_SkillManager;
			//std::map<unsigned int, maths::vec2> m_PlayerLocation;
			RegularMapUpdate m_PlayersLocationStruct;
			std::map<unsigned int, MonsterStateUpdate> m_Monsters;
		public:
			ClientHandler(ui::UiManager * ui_manager, game::NetCharManager * net_char_manager, 
				game::MonsterManager * monster_manager,
				item::ItemManager * item_manager, skills::SkillManager * skill_manager);

			~ClientHandler();
			void closeConnection();

			void Update(PlayerStateUpdateMsg psu);

		private:
			void bindFunctionToChar(char bytecode, void(ClientHandler::*fptr)())
			{
				//std::function<void()> val = std::bind(fptr, this);
				//std::function<void()> val = [&]() {(this->*fptr)(); };
				//m_DistTable.insert(std::pair<char, std::function<void()>>(bytecode, val));
				m_DistTable.insert(std::pair<char, void(ClientHandler::*)()>(bytecode, fptr));
			}
			void Send();
			void Send(BufferType messgae_type, BufferType * data, int size);
			void sendStateUpdate(PlayerStateUpdateMsg char_state);

			void handleKeepAlive();

			void receiveData();

			//void addNewPlayerToMap(BufferType * buffer);
			void addNewPlayerToMap();
			void updatePlayersLocation();
			void loadMobsData();
			void updateMobData();
			//void loadCurrentMapPlayers(BufferType * buffer);
			void loadCurrentMapPlayers();
			void recvMobHit();
			void recvMobDied();
			void recvStartDialog();
			void recvPlayerUseSkillE(); //Effect
			void recvPlayerUseSkillA(); //Attack
			void recvPickItem();
			void recvDropItem();
			void recvDroppedItem();
			void recvExpireItem();
			void recvAddItem();
			void recvIncreaseSkill();
			void recvPlayerSay();
			void recvSetQuestAsInProgress();
		public:
			void sendAttackPacket(MonsterHit monster_damage);
			void sendNpcClick(unsigned int npc_id);
			void sendQuestProgress(unsigned int npc_id, unsigned int chat_id);
			void sendQuestAccepted(unsigned int npc_id, unsigned int quest_id);
			void sendCharGotHit(unsigned int new_hp);
			void sendCharUseSkillE(unsigned int skill_id, unsigned int new_mp);
			void sendCharUseSkillA(unsigned int skill_id, std::vector<MonsterHit> monsters_hit);
			void sendPickItem(unsigned int item_id);
			void sendIncreaseSkill(unsigned int skill_id) override;
			void sendItemWore(SRL::EquipItemType item_type, unsigned int item_loc) override;
			void sendSwitchInventoryItems(unsigned int item_loc1, unsigned int item_loc2, unsigned int tab_index) override;
			void sendChatMsg(std::string) override;
			PlayerData getPlayerData() const { return m_PlayerData; }
		};
	}
}
