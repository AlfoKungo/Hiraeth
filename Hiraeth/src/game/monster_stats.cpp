#include "monster_stats.h"

namespace hiraeth {
	namespace game {
		MonsterStats::MonsterStats()
			: Stats(stats.MaxHp, stats.Hp, stats.Speed, stats.Jump),
		stats{ "Slime", 5, 4444, 400, 400, 250, 250, 15, 40,
						   40, 40, 30, 30, 20, 20, 10, 100}
		{
		}

		MonsterStats::MonsterStats(SRL::MonsterStatsStruct r_stats)
			: Stats(stats.MaxHp, stats.Hp, stats.Speed, stats.Jump),
		stats(r_stats)
		{
		}

		Damage MonsterStats::getDamage() const
		{
			return Damage{ stats.Attack, stats.WeaponAcc };
		}

		void MonsterStats::causeDamage(Damage damage)
		{
			stats.Hp -= damage.RawDamage;
		}
	}
}