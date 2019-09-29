#pragma once
#include "srl/equip_item_data.h"
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Checks
{
	static void create_equip_data()
	{
		const std::string SRL_TYPE{ "equip" };
		{
			SRL::EquipItemInfo info{
				{
					{SRL::EquipItemDataType::AttackPower, 25},
					{SRL::EquipItemDataType::IntInc, 15},
					{SRL::EquipItemDataType::LukInc, 15},
				},
				{"Staff", SRL::Equip, "Gives Magic Damage"}, 
				SRL::EquipItemType::Weapon};
			std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::EquipItemInfo info{
				{
					{SRL::EquipItemDataType::AttackPower, 25},
					{SRL::EquipItemDataType::StrInc, 15},
					{SRL::EquipItemDataType::DexInc, 15},
				},
				{"Dagger", SRL::Equip, "Gives Attack Damage"}, 
				SRL::EquipItemType::Weapon};
			std::ofstream data_file("data/" + SRL_TYPE + "/1/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::EquipItemInfo info{
				{
					{SRL::EquipItemDataType::DexInc, 15},
				},
				{"Chief Hat", SRL::Equip, "Gives Swag"},
				SRL::EquipItemType::Hat};
			std::ofstream data_file("data/" + SRL_TYPE + "/2/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::EquipItemInfo info{
				{
					{SRL::EquipItemDataType::LukInc, 15},
				},
				{"Robe", SRL::Equip, "Gives Extra SWAGGG"}, 
				SRL::EquipItemType::Top};
			std::ofstream data_file("data/" + SRL_TYPE + "/3/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::EquipItemInfo info{
				{
					{SRL::EquipItemDataType::DexInc, 15},
				},
				{"Zakum Hat", SRL::Equip, "Gives Swag"},
				SRL::EquipItemType::Hat};
			std::ofstream data_file("data/" + SRL_TYPE + "/4/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
	}
}
