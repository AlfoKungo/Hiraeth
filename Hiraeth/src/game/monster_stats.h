#pragma once

#include "stats.h"

namespace hiraeth {
	namespace game {

		struct MonsterStatsStruct
		{
			std::string Name;
			unsigned int Level;
			unsigned int Exp;
			unsigned int Hp, Mp;
			unsigned int Attack, CritRate;
			unsigned int WeaponDef, MagicDef;
			unsigned int WeaponAcc, MagicAcc;
			unsigned int WeaponAvd, MagicAvd;
			unsigned int Speed, Jump;
		};
		
		class MonsterStats : public Stats
		{
		public:
			MonsterStatsStruct m_Stats;
			MonsterStats();
			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
		};

	}
}