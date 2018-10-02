#pragma once
#include <vector>
#include <fstream>
#include "item.h"
#include "graphics\layers\layer.h"
#include "graphics\texture_manager.h"
#include "basic/EventManager.h"
#include "item_data_manager.h"
#include "UI/ui_inventory.h"
#include "use_item.h"
#include "equip_item.h"
#include "UI/ui_equip.h"

namespace hiraeth {
	namespace item {

		class ItemManager: public Updatable
		{
		private:
			const std::vector<physics::FootHold>& m_FootHolds;
			graphics::Layer<Item> m_DroppedItems;
			std::vector<Item*> m_InventoryItems;
			ui::UiInventory * m_Inventory;
		public:
			ItemManager(const std::vector<physics::FootHold>& foot_holds, 
				ui::UiInventory * inventory, ui::UiEquip * equip);
			void draw() const;
			void update() override;
			void dropItem(maths::vec2 pos, unsigned int item_id);
			void mapChanged();
			Item * getItem(maths::vec2 pos);
		private:
		};
	}
}