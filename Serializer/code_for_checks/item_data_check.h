#pragma once
#include "srl/item_data.h"
#include <cereal/archives/json.hpp>
#include <fstream>


namespace Checks
{
	static void create_item_data()
	{
		const std::string SRL_TYPE{ "item" };
		{
			SRL::UseItemInfo info{ {}, {"Slimer", SRL::Use, "Gives Magic Damage"}};
			info.item_properties[SRL::UseItemDataType::Int] = 15;
			std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::UseItemInfo info{ {}, {"Mushroomer", SRL::Use, "Gives Attack Damage"}};
			info.item_properties[SRL::UseItemDataType::Str] = 10;
			std::ofstream data_file("data/" + SRL_TYPE + "/1/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::UseItemInfo info{ {}, {"Wing", SRL::Use, "Increase Speed for 30 seconds"}};
			info.item_properties[SRL::UseItemDataType::Speed] = SRL::TimedValue{ 15, 30 };
			std::ofstream data_file("data/" + SRL_TYPE + "/2/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::UseItemInfo info{ {}, {"Red Potion", SRL::Use, "Increase health by 50 health points"}};
			info.item_properties[SRL::UseItemDataType::hpInc] = 50;
			info.item_properties[SRL::UseItemDataType::stack] = 99;
			std::ofstream data_file("data/" + SRL_TYPE + "/3/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::UseItemInfo info{ {}, {"Blue Potion", SRL::Use, "Increase mana by 50 mana points"}};
			info.item_properties[SRL::UseItemDataType::mpInc] = 50;
			info.item_properties[SRL::UseItemDataType::stack] = 99;
			std::ofstream data_file("data/" + SRL_TYPE + "/4/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
	}
}