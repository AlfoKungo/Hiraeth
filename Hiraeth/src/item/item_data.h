#pragma once
#include <string>

namespace hiraeth {
	namespace item {

#define PICK_UP_TIME 5.5f
		enum Tab {
			Equip = 0,
			Use = 1,
			SetUp = 2,
			Etc = 3,
			Cash = 4,
		};

		struct ItemInfo
		{
			Tab type;
			template<class A> void serialize(A& ar) {
				ar(type);
			}
		};
		struct ItemData
		{
			std::string item_name;
			ItemInfo item_info;
			template<class A> void serialize(A& ar) {
				ar(item_name, item_info);
			}
		};
	}
}