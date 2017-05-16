#include "monster_stats.h"

namespace hiraeth {
	namespace game {
		MonsterStats::MonsterStats()
			: m_Stats{ "Slime", 5, 4444, 400, 250, 15, 40,
						   40, 40, 30, 30, 20, 20, 100, 100}
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