#pragma once

#include "stats.h"
#include "srl/monster_data.h"

namespace hiraeth {
	namespace game {

		class MonsterStats : public Stats
		{
		public:
			SRL::MonsterStatsStruct m_Stats;
			MonsterStats();
			explicit MonsterStats(SRL::MonsterStatsStruct r_stats);
			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
		};

	}
}