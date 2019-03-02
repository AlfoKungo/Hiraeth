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

#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define SERVER "127.0.0.1"  //ip address of udp server
const int BUFLEN = 512;  //Max length of buffer
const int PORT = 8888;   //The port on which to listen for incoming data

namespace hiraeth {
	namespace network {

		class ClientHandler
		{
		private:
			const float KA_TIMEOUT = 1.0f, KA_LOSS_TIMEOUT = 3.0f;
			SOCKET m_Handle;
			unsigned int m_Id{0};
			struct sockaddr_in m_SiOther {};
			int slen = sizeof(m_SiOther), m_SendSize;
			//BufferType m_RcvBuffer[BUFLEN], m_SendBuffer[BUFLEN];
			char m_RcvBuffer[BUFLEN], m_SendBuffer[BUFLEN];
			ATimer m_KATimer{ KA_TIMEOUT }, m_KALossTimer{ KA_LOSS_TIMEOUT };

			game::NetCharManager * m_NetCharManager;
			game::MonsterManager * m_MonsterManager;
			//std::map<unsigned int, maths::vec2> m_PlayerLocation;
			RegularMapUpdate m_PlayersLocationStruct;
			std::map<unsigned int, MonsterStateUpdate> m_Monsters;
		public:
			ClientHandler(game::NetCharManager * net_char_manager, game::MonsterManager * monster_manager);

			~ClientHandler();
			void closeConnection();

			void Update(PlayerStateUpdateMsg psu);

		private:
			void Send();
			void Send(BufferType messgae_type, BufferType * data, int size);
			void sendStateUpdate(PlayerStateUpdateMsg char_state);

			void handleKeepAlive();

			void receiveData();

			void addNewPlayerToMap(BufferType * buffer);
			void updatePlayersLocation();
			void loadMobsData();
			void updateMobData();
			void loadCurrentMapPlayers(BufferType * buffer);
			void MobHit();
			void MobDied();
		public:
			void sendAttackPacket(MonsterDamage monster_damage);

		};
	}
}
