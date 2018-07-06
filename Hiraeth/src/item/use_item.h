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
				std::string stat_type = item_data.item_info.stats.substr(0, 3);
				std::string stat_value = item_data.item_info.stats.substr(4);
				if (stat_type == "spd")
					std::cout << "added speed" << "\n";
				else
					std::cout << "added nothing" << "\n";
			}


			
			void update() override
			{
				Item::update();
			}

		};
	}
}