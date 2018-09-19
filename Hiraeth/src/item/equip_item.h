#pragma once
#include "item.h"


namespace hiraeth {
	namespace item {
		enum EquipType
		{
			Weapon,
			Top,
		};
		class EquipItem : public Item
		{
		private:
			unsigned int m_ReqLvl;
			EquipType m_EquipType;
		public:
			EquipItem(maths::vec2 pos, SRL::ItemData item_data, const std::vector<physics::FootHold>& foot_holds)
				: Item(pos, item_data, foot_holds)
			{

			}


		};
	}
}