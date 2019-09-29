#pragma once
//#include "item.h"


namespace hiraeth {
	namespace item {

		class UseItem : public ItemHold
		{
		private:
			SRL::ItemPropertiesMap m_PropertiesMap;
			unsigned int m_Amount{ 0 };
		public:
			UseItem(unsigned int item_type_id, SRL::UseItemData item_data,
				unsigned int amount)
				: ItemHold(item_type_id, item_data.info.basic_item_info,
					item_data.texture_data),
				//item_data.texture_data, foot_holds, item_id),
				m_PropertiesMap(item_data.info.item_properties),
				m_Amount(amount)
			{
			}
			
			void update() override
			{
				ItemHold::update();
			}

			SRL::ItemPropertiesMap * getItemProperties() { return &m_PropertiesMap; }
		};
	}
}