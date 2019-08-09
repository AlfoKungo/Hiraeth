#pragma once
#include "srl/quest_data.h"
#include <cereal/archives/json.hpp>
#include "cereal/types/tuple.hpp"
#include <fstream>

namespace Checks
{
	static void create_quest_data()
	{
		const std::string SRL_TYPE{ "quest" };
		{
			SRL::QuestData quest{"first",
				{
					{SRL::QuestProps::KillAmount, SRL::QuestDouble{0, 20}},
					{SRL::QuestProps::KillAmount, SRL::QuestDouble{1, 10}},
					{SRL::QuestProps::ExpReward, 5000},
				}
			};
			std::ofstream data_file("data/" + SRL_TYPE + "/0.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(quest));
		}
		{
			SRL::QuestData quest{"second",
				{
					{SRL::QuestProps::KillAmount, SRL::QuestDouble{1, 15}},
					{SRL::QuestProps::MoneyReward, 3000},
				}
			};
			std::ofstream data_file("data/" + SRL_TYPE + "/1.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(quest));
		}
	}
}