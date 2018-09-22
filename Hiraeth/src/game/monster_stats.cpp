#include "monster_stats.h"

namespace hiraeth {
	namespace game {
		MonsterStats::MonsterStats()
			: Stats(m_Stats.MaxHp, m_Stats.Hp, m_Stats.Speed, m_Stats.Jump),
		m_Stats{ "Slime", 5, 4444, 400, 400, 250, 250, 15, 40,
						   40, 40, 30, 30, 20, 20, 10, 100}
		{
		}

		MonsterStats::MonsterStats(SRL::MonsterStatsStruct r_stats)
			: Stats(m_Stats.MaxHp, m_Stats.Hp, m_Stats.Speed, m_Stats.Jump),
		m_Stats(r_stats)
		{
		}

		Damage MonsterStats::getDamage() const
		{
			return Damage{ m_Stats.Attack, m_Stats.WeaponAcc };
		}

		void MonsterStats::causeDamage(Damage damage)
		{
			m_Stats.Hp -= damage.RawDamage;
		}
	}
}