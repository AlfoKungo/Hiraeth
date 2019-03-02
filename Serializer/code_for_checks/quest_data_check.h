#pragma once
#include "srl/quest_data.h"
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Checks
{
	static void create_quest_data()
	{
		const std::string SRL_TYPE{ "skills" };
		{
			SRL::QuestData quest{
				{
					//{SRL::QuestProps::KillAmount, SRL::QuestDouble{0, 20}},
					{SRL::QuestProps::ExpReward, 5000},
				}
			};
			std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(quest));
		}
	}
}