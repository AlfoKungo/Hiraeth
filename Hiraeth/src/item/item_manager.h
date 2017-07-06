#pragma once
#include <vector>
#include <fstream>
#include "item.h"
#include "graphics\layers\layer.h"
#include "graphics\texture_manager.h"
#include "basic\EventManager.h"
#include "item_data_manager.h"

namespace hiraeth {
	namespace item {

		class ItemManager: public Updatable
		{
		private:
			const std::vector<physics::FootHold>& m_FootHolds;
			graphics::Layer<Item> m_DroppedItems;
			std::vector<Item*> m_InventoryItems;
		public:
			ItemManager(const std::vector<physics::FootHold>& foot_holds);
			void draw() const;
			void update() override;
			void dropItem(const char* name, maths::vec2 pos);
			void mapChanged();
			Item * getItem(maths::vec2 pos);
			const std::vector<Item*>& getInventoryItems() { return m_InventoryItems; }
		private:
			static void serialize_data();
		};
	}
}