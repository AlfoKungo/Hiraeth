#pragma once
#include "item_data.h"

namespace SRL {
	
	enum EquipItemType
	{
		Weapon,
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
	};

	enum EquipItemDataType
	{
		// *** Ints *** //
		// Required stats
		ReqLvl,
		ReqStr,
		ReqDex,
		ReqLuk,
		ReqInt,
		// Increased stats
		AttackPower,
		StrInc,
		DexInc,
		LukInc,
		IntInc,
		MaxHpInc,
		MaxMpInc,
	};

	using EquipPropertiesMap = std::map<EquipItemDataType, std::variant<int, std::string>>;
	using EquipPropertiesMapPair = std::pair<EquipItemDataType, std::variant<int, std::string>>;

	struct EquipItemInfo
	{
		EquipPropertiesMap equip_item_properties;
		BasicItemInfo item_info;
		EquipItemType equip_item_type;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(equip_item_properties), CEREAL_NVP(item_info), CEREAL_NVP(equip_item_type));
		}
	};

	struct EquipItemData
	{
		EquipItemInfo info;
		TextureData texture_data;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(info), CEREAL_NVP(texture_data));
		}
	};
}