#pragma once
#include "socket_handler.h"
#include <queue>
#include "srl/map_data.h"
#include "mob_manager.h"
#include "net/srl_funcs.h"
#include "net/client_funcs.h"
#include "net/server_funcs.h"
#include "net/net_msgs.h"
#include "db_client.h"
#include "item_funcs.h"

namespace hiraeth {
	namespace network {
		struct ItemExpirer
		{
			unsigned int item_id;
			//float expire_time;
			ATimer expire_timer;
		};
		const float EXPIRE_TIME = 20.0f;


		struct MapHolder
		{
			std::map<unsigned int, ItemDropData> items_dropped{};
			std::map<unsigned int, PlayerStateUpdateMsg> players_state{};
			MobManager mob_manager;
			std::map<unsigned int, SRL::EquipPropertiesMap> m_EquipsStats{};
			std::map<IdType, decltype(PlayerData::equips_char)> players_equips{};
		private:
			//Socket m_Socket;
			std::queue<ItemExpirer> m_ExpiringQueue{};
			unsigned int m_ItemsIdCounter{ 0 };

		public:
			MapHolder(unsigned int map_id) :
				mob_manager{ map_id }
			{
			}
			MapHolder()
				: mob_manager{0}
			{	throw "Wrong Constructor called";	}
			ItemDropData addDrop(unsigned int item_type_id, unsigned int item_kind, 
				maths::vec2 pos);
			//ItemDropData addEquipDrop(unsigned int item_type_id, unsigned int item_kind,
			//	maths::vec2 pos, SRL::EquipPropertiesMap equip_info);
			ItemDropData addDrop(SRL::EquipDbStruct equip_info,
				maths::vec2 pos);
			void findExpiredItems();
			void sendItemExpired(unsigned int item_id);
			void sendNewPlayerInMap(unsigned int new_char_index);
			void sendMobsData(Address sender);
			void sendMobsUpdate(unsigned int mob_id, MobMoveCommand mmc);
			void updateMobManager();
			void sendUpdateLocationToAll(Address sender);
			void sendPlayerLeftMap(unsigned int player_id);
			void sendWearUpdate(IdType player_id, 
			decltype(PlayerData::equips_char) char_equips);

			void sendDataToAllClients(unsigned int size);
			void sendDataToAllClientsInMapExcept(unsigned int size, unsigned int exclude_id);
		};
	}
}
