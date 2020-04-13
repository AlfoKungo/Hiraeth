#pragma once
#include <string>
#include "srl/equip_item_data.h"

namespace hiraeth
{
	class NetworkHandler
	{
	public:
		virtual ~NetworkHandler() = default;
		virtual void sendIncreaseSkill(unsigned skill_id) = 0;
		virtual void sendItemWore(SRL::EquipItemType item_type, unsigned int item_loc) = 0;
		virtual void sendSwitchInventoryItems(unsigned int item_loc1, unsigned int item_loc2, unsigned int tab_index) = 0;
		virtual void sendChatMsg(std::string msg) = 0;
	};
	
	class NetworkManager
	{
	private:
		static NetworkHandler * _Instance;
	public:
		static void setHandler(NetworkHandler * instance)
		{
			_Instance = instance;
		}
		static NetworkHandler * Instance()
		{
			return _Instance;
		}
	};
}
