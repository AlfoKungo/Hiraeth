#pragma once

#include "texture_data.h"
#include <string>
#include <variant>
#include "cereal/types/variant.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/tuple.hpp"
#include <tuple>

namespace SRL {

	enum ItemTab {
		Equip = 0,
		Use = 1,
		SetUp = 2,
		Etc = 3,
		Cash = 4,
	};

	enum EquipableType
	{
		Hat,
		EyeAcc,
		FaceAcc,
		Top,
		Bottom,
		Cape,
		Glove,
		Shoes,
		Earrings,
		Shoulder,
		Weapon,
	};

	struct TimedValue
	{
		int value, duration;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(value), CEREAL_NVP(duration));
		}
	};

	enum ItemDataType
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

	//typedef std::tuple<float, BYTE> TimedValue;

	//typedef std::map<ItemDataType, std::variant<int, std::string>> ItemPropertiesMap;
	//typedef std::pair<ItemDataType, std::variant<int, std::string>> ItemPropertiesMapPair;

	typedef std::map<ItemDataType, std::variant<int, std::string, TimedValue>> ItemPropertiesMap;
	typedef std::pair<ItemDataType, std::variant<int, std::string, TimedValue>> ItemPropertiesMapPair;

	struct ItemInfo
	{
		ItemPropertiesMap item_properties;
		std::string item_name;
		ItemTab type;
		std::string item_description;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(item_properties), CEREAL_NVP(item_name), CEREAL_NVP(type), CEREAL_NVP(item_description));
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
