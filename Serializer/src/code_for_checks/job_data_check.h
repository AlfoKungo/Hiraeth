#pragma once
#include "srl/skill_data.h"
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Checks
{
	static void create_job_data()
	{
		const std::string SRL_TYPE{ "jobs" };
		{
			SRL::AllJobsData info{{
			{ SRL::Berserker, {{1,2,3}, {1,2,3}}},
			{ SRL::CrusaderKnight, {{1,2,3}, {1,2,3}}},
			{ SRL::Wizard, {{1,2,3}, {1,2,3}}},
			{ SRL::Rogue, {{1,2,3}, {1,2,3}}},
			{ SRL::Archer, {{1,2,3}, {1,2,3}}},
			{ SRL::ForestFighter, {{1,2,3}, {1,2,3}}},
				}};
			std::ofstream data_file("data/" + SRL_TYPE + "/jobsjson", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
	}
}

