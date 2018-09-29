#pragma once
#include "srl/skill_data.h"
#include <cereal/archives/json.hpp>
#include <fstream>
//#include "../../Hiraeth/src/skills/SkillIcon.h"

namespace Checks
{
	static void create_skill_data()
	{
		const std::string SRL_TYPE{ "skills" };
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::mpCon, 15},
					{SRL::SkillDataType::heal, 35},
					{SRL::SkillDataType::actTime, 1000},
					{SRL::SkillDataType::timeOut, 10},
				}, "Heal", true, 3 };
			std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::mpCon, 10},
					{SRL::SkillDataType::speed, SRL::TimedValue{ 5, 20 }},
					{SRL::SkillDataType::actTime, 1000},
					{SRL::SkillDataType::duration, 20},
				}, "Haste", true, 3 };
			std::ofstream data_file("data/" + SRL_TYPE + "/1/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::mpCon, 3},
					{SRL::SkillDataType::dmgS, "x*1.5"},
				}, "Snails", true, 3 };
			std::ofstream data_file("data/" + SRL_TYPE + "/2/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}

	}
}