#pragma once
#include "item.h"


namespace hiraeth {
	namespace item {

		class UseItem : public Item
		{
		private:
		public:
			UseItem(maths::vec2 pos, SRL::ItemData item_data, const std::vector<physics::FootHold>& foot_holds)
				: Item(pos, item_data, foot_holds)
			{

			}


			
			void update() override
			{
				Item::update();
				//m_DetailsBox.setPosition(m_Bounds.position + maths::vec2(30, -75));
			}

		};
	}
}