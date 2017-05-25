#include "character_stats.h"

namespace hiraeth {
	namespace game {
		CharacterStats::CharacterStats()
			: m_StatsStruct{ "Kanye", "Creative Genius", 43, 4039204, 2042185, 4, 45,
						   "Good Music", 430, 430, 200, 180, 9999, 3, 4, 9, 9, 4 , false},
			m_DetailsStruct{ 100, 40, 10, 10, 20, 20, 30, 30, 100, 100 }
		{
		}

		Damage CharacterStats::getDamage() const
		{
			return Damage{ m_StatsStruct.Str * m_DetailsStruct.Attack, m_DetailsStruct.WeaponAcc };
		}

		void CharacterStats::causeDamage(Damage damage)
		{
			m_StatsStruct.Hp -= damage.RawDamage;
			m_StatsStruct.is_changed = true;
		}
	}
}