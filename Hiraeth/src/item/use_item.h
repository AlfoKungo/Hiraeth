#pragma once
#include "item.h"


namespace hiraeth {
	namespace item {

		class UseItem : public Item
		{
		private:
			SRL::ItemPropertiesMap m_PropertiesMap;
		public:
			UseItem(maths::vec2 pos, SRL::UseItemData item_data, const std::vector<physics::FootHold>& foot_holds,
				unsigned int item_id)
				: Item(pos, item_data.info.basic_item_info, item_data.texture_data, foot_holds, item_id),
				m_PropertiesMap(item_data.info.item_properties)
			{
			}
			
			void update() override
			{
				Item::update();
			}

			SRL::ItemPropertiesMap * getItemProperties() { return &m_PropertiesMap; }
		};
	}
}