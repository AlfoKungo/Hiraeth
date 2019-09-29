#pragma once
#include <vector>
#include <fstream>
#include "item.h"
#include "item_drop.h"
#include "graphics/layers/layer.h"
#include "graphics/texture_manager.h"
#include "basic/EventManager.h"
#include "item_data_manager.h"
#include "UI/ui_inventory.h"
#include "use_item.h"
#include "equip_item.h"
#include "UI/ui_equip.h"
#include "net/protocol.h"

namespace hiraeth {
	namespace item {

		class ItemManager: public Updatable
		{
		private:
			const std::vector<physics::FootHold>& m_FootHolds;
			//graphics::Layer<Item> m_DroppedItems;
			graphics::Layer<ItemDrop> m_DroppedItems;
			//std::map<unsigned int, Item*> m_DroppedItemsMap;
			std::map<unsigned int, ItemDrop*> m_DroppedItemsMap;
			//std::vector<Item*> m_InventoryItems;
			//std::vector<ItemHold*> m_InventoryItems;
			ui::UiInventory * m_Inventory;
			ui::UiEquip * m_Equip;
			//unsigned int m_Counter{ 0 };
		public:
			ItemManager(const std::vector<physics::FootHold>& foot_holds, 
				ui::UiInventory * inventory, ui::UiEquip * equip);
			void draw() const;
			void update() override;
			void setInvEquip(decltype(network::PlayerData::inv_equip) equips);
			void setInvUse(decltype(network::PlayerData::inv_use) items);
			void setEquipsChar(decltype(network::PlayerData::equips_char) equips);
			//void dropItem(maths::vec2 pos, unsigned int item_id);
			void dropItem(unsigned int item_id, unsigned int item_type_id, unsigned int item_kind, maths::vec2 pos);
			void addItemToInv(unsigned int item_kind, unsigned int item_loc, unsigned int item_id); 
			void startExpiring(unsigned int item_id);
			void mapChanged();
			ItemDrop * getItem(maths::vec2 pos);
			//Item * getItem(unsigned int item_id) { return m_DroppedItemsMap[item_id]; }
			ItemDrop * getItem(unsigned int item_id) { return m_DroppedItemsMap[item_id]; }
		private:
		};
	}
}