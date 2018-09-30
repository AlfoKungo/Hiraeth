#pragma once

#include "game/stats.h"
#include "srl/monster_data.h"

namespace hiraeth {
	namespace game {

		class MonsterStats : public Stats
		{
		public:
			SRL::MonsterStatsStruct m_Stats;
			MonsterStats()
				:
				m_Stats{ "Slime", 5, 4444, 400, 400, 250, 250, 15, 40,
								   40, 40, 30, 30, 20, 20, 10, 100 }
			{

			}
			explicit MonsterStats(SRL::MonsterStatsStruct r_stats)
				:
				m_Stats(r_stats)
			{

			}

			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
			float getSpeed() const override;
			float getJump() const override;
			bool consumeMana(unsigned mp_consume) override;
			void recoverHp(unsigned heal_amount) override;
		};

	}
}