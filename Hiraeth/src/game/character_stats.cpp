#include "character_stats.h"

namespace hiraeth {
	namespace game {
		CharacterStats::CharacterStats()
			: Stats(m_StatsStruct.MaxHp, m_StatsStruct.MaxHp, m_DetailsStruct.Speed, m_DetailsStruct.Jump),
		m_StatsStruct{ "Kanye", "Creative Genius", 43, 4039204, 2042185, 4, 45,
						   "Good Music", 430, 430, 200, 180, 9999, 3, 4, 9, 9, 4 , false},
			m_DetailsStruct{ 40, 40, 10, 10, 20, 20, 30, 30, 25, 13 }
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->createEvent(StatsUpdate);
		}

		Damage CharacterStats::getDamage() const
		{
			return Damage{ m_StatsStruct.Str * m_DetailsStruct.Attack, m_DetailsStruct.WeaponAcc };
		}

		void CharacterStats::causeDamage(Damage damage)
		{
			if (m_StatsStruct.Hp > damage.RawDamage)
				m_StatsStruct.Hp -= damage.RawDamage;
			
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->execute(StatsUpdate);
		}
	}
}