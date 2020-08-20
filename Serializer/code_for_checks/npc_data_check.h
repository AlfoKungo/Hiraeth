#pragma once
#include <cereal/archives/json.hpp>
#include <string>
#include <fstream>
#include "srl/npc_data.h"

namespace Checks
{
	static void create_npc_data()
	{
		const std::string SRL_TYPE{ "npc" };
		{
			SRL::NpcInfo info{
				"Chief Guatamala",
				0,
				0.0,
				{"Hey, care for some CDs nigga?", "May I interest you in\nsome Kanye?"},
			{
				{0,
					{{"They tryna find TuPaC",{SRL::RT_NEXT}},
					{"Don't let them find tupac",{SRL::RT_NEXT}},
					//{"La da da da",{2,1}}}}
					{"La da da da",{SRL::RT_CANCEL,SRL::RT_ACCEPT}}}},
			{1,
					{{"I'm waiting for it you bitch ass nigga",{SRL::RT_NEXT}}}},
			{2,
					{{"Aight you cool\n there's you money",{SRL::RT_RECEIVE_REWARD}}}},
			{3,
					{{"Aight you cool\n there's you money",{SRL::RT_NEXT}}}}
			}
			};
			std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
	}
}
