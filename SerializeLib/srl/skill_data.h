#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include "texture_data.h"

namespace SRL
{
	struct SkillInfo
	{
		std::string name;
		float mp_con;
		float damage;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(mp_con), CEREAL_NVP(damage));
		}
	};

	struct SkillData
	{
		SkillInfo skill_info;
		TextureData texture_data;
		unsigned int skill_frames_amount = 13;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(skill_info), CEREAL_NVP(texture_data));
		}
	};
}
