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
					//{SRL::SkillDataType::mpCon, 15},
					{SRL::SkillDataType::mpCon, "15-x*5"},
					{SRL::SkillDataType::heal, 35},
					//{SRL::SkillDataType::heal, "35+10*x"},
					{SRL::SkillDataType::actTime, 1000},
					{SRL::SkillDataType::timeOut, 10},
				}, "Heal", 3 , SRL::active_buff};
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
				}, "Haste", 3, SRL::active_buff};
			std::ofstream data_file("data/" + SRL_TYPE + "/1/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::mpCon, 3},
					{SRL::SkillDataType::damage, "x*1.5"},
					{SRL::SkillDataType::proj_range, "600"},
				}, "Snails", 3, SRL::active_attack_proj_targeted };
			std::ofstream data_file("data/" + SRL_TYPE + "/2/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::inc_max_mp, "x"},
					{SRL::SkillDataType::inc_max_mp_per_lvl, "20 + 5*x"},
					{SRL::SkillDataType::inc_crit_chance, "3"},
				}, "MP Boost", 20, SRL::passive_s };
			std::ofstream data_file("data/" + SRL_TYPE + "/3/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::mpCon, "30 - 2*x"},
					{SRL::SkillDataType::move_x, "110 + 12*x"},
					{SRL::SkillDataType::move_y, "250 + 5*x"},
				}, "Teleport", 5, SRL::active_move };
			std::ofstream data_file("data/" + SRL_TYPE + "/4/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::lifesteal, "20 + 2*x"},
				}, "Life Stealer", 10, SRL::passive_effect };
			std::ofstream data_file("data/" + SRL_TYPE + "/5/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::disable_lifesteal, "100 - x"},
					{SRL::SkillDataType::spd_by_mhealth, "x"},
					{SRL::SkillDataType::jmp_by_mhealth, "x"},
					{SRL::SkillDataType::inc_str, "5*x"},
					{SRL::SkillDataType::atk_spd_by_mhealth, "x"},
				}, "State Enhancer", 10, SRL::passive_effect };
			std::ofstream data_file("data/" + SRL_TYPE + "/6/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::atk_by_mhealth, "x"},
				}, "Battle Rage", 10, SRL::active_move };
			std::ofstream data_file("data/" + SRL_TYPE + "/7/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::mpCon, "15 - x"},
					{SRL::SkillDataType::dmg, "130 + 12*x"},
				}, "Power Swing", 10, SRL::active_attack_swing };
			std::ofstream data_file("data/" + SRL_TYPE + "/8/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::mpCon, "30 - 2*x"},
					{SRL::SkillDataType::move_x, "110 + 12*x"},
					{SRL::SkillDataType::move_y, "250 + 5*x"},
					{SRL::SkillDataType::dmg, "100 + 5*x"},
				}, "Rush Dash", 10, SRL::active_move };
			std::ofstream data_file("data/" + SRL_TYPE + "/9/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}
		{
			SRL::SkillInfo info{
				{
					{SRL::SkillDataType::inc_max_mp_perc, "x"},
					{SRL::SkillDataType::inc_max_mp_per_lvl, "30 + x"},
					{SRL::SkillDataType::inc_mp_reg, "x"},
				}, "Mana Mash", 10, SRL::active_move };
			std::ofstream data_file("data/" + SRL_TYPE + "/10/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(data_file);
			arout(CEREAL_NVP(info));
		}

	}
}