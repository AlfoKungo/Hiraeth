#include "hrth_pch.h"
#include "monster_stats.h"

namespace hiraeth {
	namespace game {

		Damage MonsterStats::getDamage() const
		{
			return Damage{ m_Stats.Attack, m_Stats.WeaponAcc };
		}

		void MonsterStats::causeDamage(Damage damage)
		{
			m_Stats.Hp -= damage.RawDamage;
		}

		float MonsterStats::getSpeed() const
		{
			return m_Stats.Speed;
		}

		float MonsterStats::getJump() const
		{
			return m_Stats.Jump;
		}

		bool MonsterStats::consumeMana(unsigned mp_consume)
		{
			m_Stats.Mp -= mp_consume;
			return true;
		}

		void MonsterStats::recoverHp(unsigned heal_amount)
		{
			m_Stats.Hp += heal_amount;
		}
	}
}