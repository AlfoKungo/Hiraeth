#pragma once

#include "texture_data.h"
#include <string>
#include <variant>
#include "cereal/types/variant.hpp"
#include "cereal/types/map.hpp"
#include "types.h"

namespace SRL {

	enum ItemTab {
		Equip = 0,
		Use = 1,
		SetUp = 2,
		Etc = 3,
		Cash = 4,
	};

	enum UseItemDataType
	{
		// Ints
		Str,
		Dex,
		Luk,
		Int,
		hpInc,
		mpInc,
		stack,
		// TimedValue
		Speed,
		Jump,
	};

	typedef std::map<UseItemDataType, std::variant<int, std::string, TimedValue>> ItemPropertiesMap;
	typedef std::pair<UseItemDataType, std::variant<int, std::string, TimedValue>> ItemPropertiesMapPair;

	struct BasicItemInfo
	{
		std::string item_name;
		ItemTab type;
		std::string item_description;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_name), CEREAL_NVP(type), CEREAL_NVP(item_description));
		}
	};

	//struct ItemInfo
	//{
	//	ItemPropertiesMap item_properties;
	//	BasicItemInfo basic_item_info;
	//	//std::string item_name;
	//	//ItemTab type;
	//	//std::string item_description;
	//	template<class A> void serialize(A& ar) {
	//		//ar(CEREAL_NVP(item_properties), CEREAL_NVP(item_name), CEREAL_NVP(type), CEREAL_NVP(item_description));
	//		ar(CEREAL_NVP(item_properties), CEREAL_NVP(basic_item_info));
	//	}
	//};

	struct UseItemInfo
	{
		ItemPropertiesMap item_properties;
		BasicItemInfo basic_item_info;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_properties), CEREAL_NVP(basic_item_info));
		}
	};

	struct UseItemData
	{
		UseItemInfo item_info;
		TextureData texture_data;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_info), CEREAL_NVP(texture_data));
		}
	};

	//struct ItemData
	//{
	//	ItemInfo item_info;
	//	TextureData texture_data;
	//	template<class A> void serialize(A& ar) {
	//		ar(CEREAL_NVP(item_info), CEREAL_NVP(texture_data));
	//	}
	//};
}
