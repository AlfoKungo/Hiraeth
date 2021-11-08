#pragma once
#include "item_data.h"
#include "maths/maths.h"

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

	enum AnimationState
	{
		as_Default,

	};

	enum EquipReqEnum
	{
		eReqLvl,
		eReqStr,
		eReqDex,
		eReqWit,
		eReqMed,
		eReqJob,
	};
	struct EquipReqsStruct
	{
		unsigned int Lvl{}, Str{}, Int{}, Dex{}, Wit{}, Med{};
		unsigned char Job{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(Lvl), CEREAL_NVP(Str), CEREAL_NVP(Int), CEREAL_NVP(Dex),
				CEREAL_NVP(Wit), CEREAL_NVP(Med), CEREAL_NVP(Job));
		}
	};

	using EquipPropertiesMap = std::map<EquipItemDataType, std::variant<int, std::string>>;
	using EquipPropertiesMapPair = std::pair<EquipItemDataType, std::variant<int, std::string>>;

	struct EquipReqs
	{
		std::map<EquipReqEnum, int> reqs_map;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(reqs_map));
		}
	};
	struct EquipDbStruct
	{
		unsigned int equip_id;
		EquipPropertiesMap equip_item_properties;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(equip_id), CEREAL_NVP(equip_item_properties));
		}

	};
	struct RandomStruct
	{
		SRL::EquipItemDataType stat_type;
		unsigned int default, plus_minus;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(stat_type), CEREAL_NVP(default), CEREAL_NVP(plus_minus));
		}
	};
	struct EquipItemInfo
	{
		//EquipPropertiesMap equip_item_properties;
		BasicItemInfo item_info;
		EquipItemType equip_item_type;
		//std::map<EquipReqEnum, int> reqs_map;
		EquipReqsStruct reqs;
		std::vector<RandomStruct> equip_props;
		template<class A> void serialize(A& ar) {
			//ar(CEREAL_NVP(equip_item_properties), CEREAL_NVP(item_info), CEREAL_NVP(equip_item_type));
			ar(CEREAL_NVP(item_info), CEREAL_NVP(equip_item_type), CEREAL_NVP(reqs), CEREAL_NVP(equip_props));
		}
	};

	struct EquipTextureData
	{
		TextureData tex_data;
		maths::vec2 org;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(tex_data), CEREAL_NVP(org));
		}
	};

	using EquipTexturesMap = std::map<AnimationState, EquipTextureData>;

	struct EquipItemData
	{
		EquipItemInfo info;
		TextureData icon_texture;
		EquipTexturesMap textures;
		//std::map<EquipItemDataType, int> props_map;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(info), CEREAL_NVP(icon_texture), CEREAL_NVP(textures));
			//CEREAL_NVP(reqs_map), CEREAL_NVP(props_map));
		}
	};
}