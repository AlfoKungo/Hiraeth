#include "character_stats.h"

namespace hiraeth {
	namespace game {
		CharacterStats::CharacterStats()
			:
			//Stats(),
			m_StatsStruct{ "Kanye", "Creative Genius", 43, 4039204, 2042185, 4, 45,
							   "Good Music", 430, 430, 200, 180, 430, 200, BASE_HP_REG, BASE_MP_REG, 9999,
				3, 4, 9, 9, 4, 5 },
			m_DetailsStruct{ 40, 40, 40, 10, 10, 20, 20, 30, 30, 25, 13 }
		{

			m_BaseStats.MaxHp = m_StatsStruct.MaxHp;
			m_BaseStats.MaxMp = m_StatsStruct.MaxMp;
			m_BaseStats.HpReg = m_StatsStruct.HpReg;
			m_BaseStats.MpReg = m_StatsStruct.MpReg;
			m_BaseStats.Str = m_StatsStruct.Str;
			m_BaseStats.Dex = m_StatsStruct.Dex;
			m_BaseStats.Int = m_StatsStruct.Int;
			m_BaseStats.Wit = m_StatsStruct.Wit;
			m_BaseStats.Med = m_StatsStruct.Med;
			m_BaseStats.Attack = m_DetailsStruct.Attack;
			m_BaseStats.CritRate = m_DetailsStruct.CritRate;
			m_BaseStats.CritDmg = m_DetailsStruct.CritDmg;
			m_BaseStats.Speed = m_DetailsStruct.Speed;
			m_BaseStats.Jump = m_DetailsStruct.Jump;

			//m_StatsMap[ST_MaxHp] = m_AddOnStats.MaxHp;
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_MaxHp, m_AddOnStats.MaxHp));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_MaxMp, m_AddOnStats.MaxMp));
			//m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_HpReg, m_AddOnStats.HpReg));
			//m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_MpReg, m_AddOnStats.MpReg));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_Str, m_AddOnStats.Str));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_Dex, m_AddOnStats.Dex));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_Int, m_AddOnStats.Int));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_Wit, m_AddOnStats.Wit));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_Med, m_AddOnStats.Med));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_Speed, m_AddOnStats.Speed));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_Jump, m_AddOnStats.Jump));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_CritChance, m_AddOnStats.CritRate));
			m_StatsMap.insert(std::pair<StatsType, unsigned int&>(ST_CritDmg, m_AddOnStats.CritDmg));

			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->createEvent<>(StatsUpdate);
		}


		void CharacterStats::updateStats()
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->execute(StatsUpdate);
		}

		Damage CharacterStats::getDamage() const
		{
			Damage d{ m_StatsStruct.Str * m_DetailsStruct.Attack, m_DetailsStruct.WeaponAcc };
			for (const auto& [id, mods]: m_AttackModifiers)
			{
				for (const auto& mod : mods)
				{
					switch (mod.first)
					{
					case SRL::lifesteal:
						//m_StatsStruct.Hp += int(d.RawDamage);
					default:
						break;
					}
				}
			}
			return Damage{ m_StatsStruct.Str * m_DetailsStruct.Attack, m_DetailsStruct.WeaponAcc };
		}

		void CharacterStats::causeDamage(Damage damage)
		{
			if (m_StatsStruct.Hp > damage.RawDamage)
				m_StatsStruct.Hp -= damage.RawDamage;
			updateStats();
		}

		bool CharacterStats::consumeMana(unsigned mp_consume)
		{
			if (m_StatsStruct.Mp < mp_consume)
				return false;
			m_StatsStruct.Mp -= mp_consume;
			updateStats();
			return true;
		}

		void CharacterStats::recoverHp(unsigned heal_amount)
		{
			m_StatsStruct.Hp = std::min(m_StatsStruct.Hp + heal_amount, m_StatsStruct.MaxHp);
			updateStats();
		}

		void CharacterStats::recoverMp(unsigned heal_amount)
		{
			m_StatsStruct.Mp = std::min(m_StatsStruct.Mp + heal_amount, m_StatsStruct.MaxMp);
			updateStats();
		}

		unsigned CharacterStats::getHpPercent() const
		{
			return unsigned int(float(m_StatsStruct.Hp) / float(m_StatsStruct.MaxHp) * 100.0f);
		}

		unsigned CharacterStats::getMHpPercent() const
		{
			return unsigned int((1.0f - float(m_StatsStruct.Hp) / float(m_StatsStruct.MaxHp)) * 100.0f);
		}

		bool CharacterStats::activateUseItem(SRL::ItemPropertiesMap* item_properties)
		{

			for (SRL::ItemPropertiesMapPair element : (*item_properties))
			{
				switch (element.first)
				{
				case SRL::UseItemDataType::Str:
					break;
				case SRL::UseItemDataType::Dex:
					break;
				case SRL::UseItemDataType::Luk:
					break;
				case SRL::UseItemDataType::Int:
					break;
				case SRL::UseItemDataType::Speed:
					setTimedStat(m_DetailsStruct.Speed, std::get<SRL::TimedValue>(element.second));
					break;
				case SRL::UseItemDataType::hpInc:
					recoverHp(std::get<int>(element.second));
					break;
				case SRL::UseItemDataType::mpInc:
					recoverMp(std::get<int>(element.second));
					break;
				default:
					break;
				}
			}
			return true;
		}

		float CharacterStats::getSpeed() const
		{
			return m_DetailsStruct.Speed;
		}

		float CharacterStats::getJump() const
		{
			return m_DetailsStruct.Jump;
		}
	}
}