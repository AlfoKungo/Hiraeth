#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include "texture_data.h"
#include "cereal/types/variant.hpp"
#include <map>

namespace SRL
{
	enum SkillDataType
	{
		mpCon,
		dmg,
		dmgS,
	};

	struct SkillInfo
	{
		//std::map<SkillDataType, >
		std::map<SkillDataType, std::variant<unsigned int, std::string>> skill_properties;
		//std::map<SkillDataType, std::any> skill_properties;
		//std::map<SkillDataType, unsigned int> skill_properties;
		std::string name;
		//unsigned char skill_type;
		float mp_con;
		float damage;
		//float cooltime;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(skill_properties), CEREAL_NVP(name), CEREAL_NVP(mp_con), CEREAL_NVP(damage));
		}
	};

	struct SkillData
	{
		SkillInfo skill_info;
		TextureData texture_data;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(skill_info), CEREAL_NVP(texture_data));
		}
	};
}
