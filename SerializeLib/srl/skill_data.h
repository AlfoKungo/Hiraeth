#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/tuple.hpp>
#include "texture_data.h"
#include "cereal/types/variant.hpp"
#include "types.h"
#include <map>
#include "animation_data.h"
#include "sprite_data.h"

namespace SRL
{
	enum SkillDataType
	{
		// -- ints --
		mpCon, //skill mp consumption
		dmg,
		heal , // heal hp
		actTime, // activation time in mili-seconds
		timeOut, // in seconds
		duration,
		// -- TimedValue -- DELETE THIS
		speed, // enhance speed
		jump, // enhance speed
		// -- strings --
		// active attack
		damage, // fixed
		hit_count,
		mobs_hit,
		elemental_type,
		// proj
		proj_range,
		// active_move
		move_x, // moves on x axis
		move_y, // moves on y axis
		// passive stats
		inc_str, // increases str
		inc_dex, // increases dex
		inc_int, // increases int
		inc_wit, // increases wit
		inc_med, // increases med
		inc_str_by_perc, // increases str by percentage
		inc_dex_by_perc, // increases dex by percentage
		inc_int_by_perc, // increases int by percentage
		inc_wit_by_perc, // increases wit by percentage
		inc_med_by_perc, // increases med by percentage
		inc_max_hp, // increase max HP
		inc_max_hp_per_lvl, // increase max HP by value per level
		inc_max_mp, // increase max MP
		inc_max_mp_per_lvl, // increase max MP by value per level
		inc_hp_reg, // increase Hp regen
		inc_mp_reg, // increase Mp regen
		inc_def, // increase defense
		inc_crit_chance, // increase crit chance
		inc_crit_dmg, // increase crit damage
		inc_drop_perc, // increase drop percentage
		inc_atk_spd, 
		// attack modifier
		lifesteal, //
		disable_lifesteal,  
		// passive update-required effects
		spd_by_mhealth, // speed by minus health
		jmp_by_mhealth, // jump by minus health
		atk_spd_by_mhealth, // attack speed by minus health
		dmg_by_mhealth, // attack speed by minus health
		// Sprited Renderable
		//skillAnimation
	};
	enum SkillType
	{
		active_attack_proj_targeted,
		active_attack_proj_straight,
		active_attack_swing,
		active_buff,
		active_move,
		passive_s,
		passive_effect,
		passive_togle, 
	};

	enum SkillAnimationTypes
	{
		effectAnimation,
		hitAnimation,
		ballAnimation,
	};

	using SkillPropertiesVar = std::variant<int, std::string, TimedValue>;
	using SkillPropertiesMap = std::map<SkillDataType, SkillPropertiesVar>;
	using SkillPropertiesMapPair = std::pair<SkillDataType, SkillPropertiesVar>;

	struct SkillInfo
	{
		SkillPropertiesMap skill_properties;
		std::string name;
		//bool is_active{true};
		unsigned int max_level{1};
		SkillType skill_type{};
		template<class A> void serialize(A& ar) {
			//ar(CEREAL_NVP(skill_properties), CEREAL_NVP(name), CEREAL_NVP(is_active),
			ar(CEREAL_NVP(skill_properties), CEREAL_NVP(name),
				CEREAL_NVP(max_level), CEREAL_NVP(skill_type));
		}
	};

	struct SkillIconsData
	{
		TextureData icon;
		TextureData icon_disabled;
		TextureData icon_mouse_over;
		void load_data(const std::string& path)
		{
			icon.load_texture(path + "\\icon.png");
			icon_disabled.load_texture(path + "\\iconDisabled.png");
			icon_mouse_over.load_texture(path + "\\iconMouseOver.png");
		}
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(icon), CEREAL_NVP(icon_disabled), CEREAL_NVP(icon_mouse_over));
		}
	};

	using AnimationMap = std::map<SkillAnimationTypes, FullAnimationData>;

	struct SkillData
	{
		SkillInfo skill_info;
		SkillIconsData icon_data;
		AnimationMap animation_map;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(skill_info), CEREAL_NVP(icon_data), CEREAL_NVP(animation_map));
		}
	};

	struct JobData
	{
		std::vector<unsigned int> first_job_skills;
		std::vector<unsigned int> second_job_skills;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(first_job_skills), CEREAL_NVP(first_job_skills));
		}
	};

	enum JobsTypes
	{
		Novice,
		Berserker,
		CrusaderKnight,
		Wizard,
		Rogue,
		Archer,
		ForestFighter,
	};

	struct AllJobsData
	{
		std::map<JobsTypes, JobData> jobs_type_to_data_map;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(jobs_type_to_data_map));
		}
	};
}
