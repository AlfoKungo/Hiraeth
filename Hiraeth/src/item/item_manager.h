#pragma once
#include <vector>
#include "item.h"
#include "graphics\layers\layer.h"
#include "graphics\texture_manager.h"
#include "basic\EventManager.h"

namespace hiraeth {
	namespace item {

		class ItemManager
		{
		private:
			const std::vector<physics::FootHold>& m_FootHolds;
			graphics::Layer<Item> m_Items;
		public:
			ItemManager(const std::vector<physics::FootHold>& foot_holds);
			void update();
			void draw() const;
			void dropItem(const char* name, maths::vec2 pos);
			void mapChanged();
		};
	}
}