#pragma once

#include "stats.h"

namespace hiraeth {
	namespace game {

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
					ar(Name, Level, Exp, MaxHp, Hp, MaxMp, Mp, Attack, CritRate, WeaponDef, MagicDef,
						WeaponAcc, MagicAcc, WeaponAvd, MagicAvd, Speed, Jump);
				}
		};
		
		class MonsterStats : public Stats
		{
		public:
			MonsterStatsStruct stats;
			MonsterStats();
			explicit MonsterStats(MonsterStatsStruct r_stats);
			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
		};

	}
}