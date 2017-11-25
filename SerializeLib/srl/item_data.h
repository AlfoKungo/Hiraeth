#pragma once

#include "texture_data.h"
#include <string>

namespace SRL {

	enum Tab {
		Equip = 0,
		Use = 1,
		SetUp = 2,
		Etc = 3,
		Cash = 4,
	};

	struct ItemInfo
	{
		std::string item_name;
		Tab type;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_name), CEREAL_NVP(type));
		}
	};
	struct ItemData
	{
		ItemInfo item_info;
		TextureData texture_data;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_info), CEREAL_NVP(texture_data));
		}
	};
}