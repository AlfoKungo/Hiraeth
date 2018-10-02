#pragma once
#include "item.h"
#include "srl/equip_item_data.h"


namespace hiraeth {
	namespace item {

		class EquipItem : public Item
		{
		private:
			SRL::EquipPropertiesMap m_EquipPropertiesMap;
			SRL::EquipItemType m_EquipType;
		public:
			EquipItem(maths::vec2 pos, SRL::EquipItemData item_data,
				const std::vector<physics::FootHold>& foot_holds)
				: Item(pos, item_data.item_info.item_info, item_data.texture_data, foot_holds),
				m_EquipPropertiesMap(item_data.item_info.equip_item_properties),
				m_EquipType(item_data.item_info.equip_item_type)
			{

			}

			SRL::EquipItemType getItemType() const { return m_EquipType; }

		};
	}
}