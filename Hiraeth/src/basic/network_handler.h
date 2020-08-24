#pragma once
#include <string>
#include "net/net_msgs.h"
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
		virtual void sendQuestProgress(unsigned int npc_id, unsigned int chat_id) = 0;
		// by npc manager
		virtual void sendNpcClick(unsigned int npc_id) = 0;
		// by dialog manager
		virtual void sendQuestAccepted(unsigned int npc_id, unsigned int quest_id) = 0;
		virtual void sendReceiveReward(unsigned int npc_id, unsigned int quest_id) = 0;
		// by character
		virtual void sendCharGotHit(unsigned int new_hp) = 0;
		virtual void sendAttackPacket(network::MonsterHit mh) = 0;
		virtual void sendPickItem(unsigned int item_id) = 0;
		virtual void sendCharUseSkillE(unsigned int skill_id, unsigned int new_mp) = 0;
		virtual void sendCharUseSkillA(unsigned int skill_id, std::vector<network::MonsterHit> monsters_hit) = 0;
		// by map
		virtual void sendEnterPortal(unsigned int portal_id) = 0;
		// by net_char_manager
		virtual void sendRequestParty(unsigned int char_id) = 0;
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
