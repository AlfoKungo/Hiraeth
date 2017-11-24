#pragma once

#include <string>
#include <memory>
#include "texture_data.h"

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
		unsigned int TextureIndex;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(StatsStruct), CEREAL_NVP(TextureIndex));
		}
	};

	struct MonsterFramesAmount
	{
		unsigned int stand_frames, walk_frames, hit_frames;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(stand_frames), CEREAL_NVP(walk_frames), CEREAL_NVP(hit_frames));
		}
	};

	struct MonsterTexturesData
	{
		TextureData stand_texture, walk_texture, hit_texture;
		MonsterFramesAmount frames_amount;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(stand_texture), CEREAL_NVP(walk_texture), CEREAL_NVP(hit_texture), CEREAL_NVP(frames_amount));
		}
	};

	//struct MonsterData
	//{
	//	std::string monster_name;
	//	MonsterFramesAmount monster_frames_amount;
	//	MonsterStatsStruct stats;

	//	template<class A> void serialize(A& ar) {
	//		ar(monster_name, monster_frames_amount, stats);
	//	}
	//};
}