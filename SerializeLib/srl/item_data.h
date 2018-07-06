#pragma once

#include "texture_data.h"
#include <string>

namespace SRL {

	enum ItemTab {
		Equip = 0,
		Use = 1,
		SetUp = 2,
		Etc = 3,
		Cash = 4,
	};

	struct ItemInfo
	{
		std::string item_name;
		ItemTab type;
		std::string item_description;
		std::string stats;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_name), CEREAL_NVP(type), CEREAL_NVP(item_description), CEREAL_NVP(stats));
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