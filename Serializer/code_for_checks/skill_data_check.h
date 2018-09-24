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
			SRL::SkillInfo info{ {}, "Heal", true, 3 };
			info.skill_properties[SRL::SkillDataType::mpCon] = 15;
			info.skill_properties[SRL::SkillDataType::heal] = 35;
			info.skill_properties[SRL::SkillDataType::actTime] = 300;
			std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{ {}, "Haste", true, 3 };
			info.skill_properties[SRL::SkillDataType::mpCon] = 10;
			info.skill_properties[SRL::SkillDataType::speed] = 20;
			//info.skill_properties[SRL::SkillDataType::duration] = 120;
			std::ofstream data_file("data/" + SRL_TYPE + "/1/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{ {}, "Snails", true, 3 };
			info.skill_properties[SRL::SkillDataType::mpCon] = 5;
			info.skill_properties[SRL::SkillDataType::dmgS] = "x*1.5";
			std::ofstream data_file("data/" + SRL_TYPE + "/2/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}

	}
}
