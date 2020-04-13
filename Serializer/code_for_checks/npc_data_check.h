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
					{{"They tryna find TuPaC",{0}},
					{"Don't let them find tupac",{0}},
					//{"La da da da",{2,1}}}}
					{"La da da da",{2,1}}}}
			}
			};
			std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
	}
}
