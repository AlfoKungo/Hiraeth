#pragma once
#include "srl/skill_data.h"
#include <cereal/archives/json.hpp>
#include <fstream>
//#include "../../Hiraeth/src/skills/SkillIcon.h"

namespace Checks
{
	static void create_skill_data()
	{
		{
			SRL::SkillInfo info{ {}, "Heal", 40, 50 };
			info.skill_properties[SRL::SkillDataType::mpCon] = 50;
			info.skill_properties[SRL::SkillDataType::dmg] = 50;
			std::ofstream data_file("data/skills/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{ {}, "Haste", 10, 15, };
			info.skill_properties[SRL::SkillDataType::mpCon] = 50;
			info.skill_properties[SRL::SkillDataType::dmg] = 50;
			std::ofstream data_file("data/skills/1/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{ {}, "Snails", 30, 35 };
			info.skill_properties[SRL::SkillDataType::mpCon] = 50;
			info.skill_properties[SRL::SkillDataType::dmgS] = "x*1.5";
			std::ofstream data_file("data/skills/2/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}

	}
}
