#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include "texture_data.h"
#include "cereal/types/variant.hpp"
#include "types.h"
#include <map>
#include "animation_data.h"

namespace SRL
{
	enum SkillDataType
	{
		// ints
		mpCon, //skill mp consumption
		dmg,
		heal , // heal hp
		actTime, // activation time in mili-seconds
		timeOut, // in seconds
		duration,
		// TimedValue
		speed, //enhance speed
		jump, //enhance speed
		// strings
		dmgS,
		// Sprited Renderable
		//skillAnimation
	};

	enum SkillAnimationTypes
	{
		effectAnimation,
		hitAnimation,
		ballAnimation,
	};

	typedef std::map<SkillDataType, std::variant<int, std::string, TimedValue>> SkillPropertiesMap;
	typedef std::pair<SkillDataType, std::variant<int, std::string, TimedValue>> SkillPropertiesMapPair;

	struct SkillInfo
	{
		SkillPropertiesMap skill_properties;
		std::string name;
		bool is_active{true};
		unsigned int max_level{1};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(skill_properties), CEREAL_NVP(name), CEREAL_NVP(is_active), CEREAL_NVP(max_level));
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

	typedef std::map<SkillAnimationTypes, FullAnimationData> AnimationMap;

	struct SkillData
	{
		SkillInfo skill_info;
		SkillIconsData icon_data;
		AnimationMap animation_map;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(skill_info), CEREAL_NVP(icon_data), CEREAL_NVP(animation_map));
		}
	};
}
