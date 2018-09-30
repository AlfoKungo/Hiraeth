#pragma once

#include <string>
#include <memory>
#include "texture_data.h"
#include <cereal/types/map.hpp>
#include "maths/vec2.h"
#include <map>
#include "maths/rectangle.h"
#include "sprite_data.h"
#include "srl/animation_data.h"

namespace SRL {
	

	struct MonsterStatsStruct
	{
		std::string Name;
		unsigned int Level;
		unsigned int Exp;
		unsigned int MaxHp, Hp, MaxMp, Mp;
		unsigned int Attack, CritRate;
		unsigned int WeaponDef, MagicDef;
		unsigned int WeaponAcc, MagicAcc;
		unsigned int WeaponAvd, MagicAvd;
		float Speed, Jump;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(Name), CEREAL_NVP(Level), CEREAL_NVP(Exp),
				CEREAL_NVP(MaxHp), CEREAL_NVP(Hp), CEREAL_NVP(MaxMp),
				CEREAL_NVP(Mp), CEREAL_NVP(Attack), CEREAL_NVP(CritRate),
				CEREAL_NVP(WeaponDef), CEREAL_NVP(MagicDef), CEREAL_NVP(WeaponAcc),
				CEREAL_NVP(MagicAcc), CEREAL_NVP(WeaponAvd), CEREAL_NVP(MagicAvd),
				CEREAL_NVP(Speed), CEREAL_NVP(Jump));
		}
	};

	struct MonsterData
	{
		MonsterStatsStruct StatsStruct;
		unsigned int TextureIndex{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(StatsStruct), CEREAL_NVP(TextureIndex));
		}
	};

	struct CreatureSprites
	{
		std::map<MoveState, FullAnimationData> sprited_data;
		maths::vec2 hit_box;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(sprited_data), CEREAL_NVP(hit_box));
		}
	};

	struct MonsterTexturesData
	{
		//TextureData stand_texture, walk_texture, hit_texture;
		CreatureSprites creature_sprites;
		//std::map<MoveState, TextureData> textures_dict;
		template<class A> void serialize(A& ar) {
			//ar(CEREAL_NVP(stand_texture), CEREAL_NVP(walk_texture), CEREAL_NVP(hit_texture), CEREAL_NVP(frames_amount));
			ar(CEREAL_NVP(creature_sprites));
		}
	};
}
