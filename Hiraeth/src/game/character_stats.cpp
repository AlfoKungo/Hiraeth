#include "character_stats.h"

namespace hiraeth {
	namespace game {
		CharacterStats::CharacterStats()
			:
			//Stats(),
			m_StatsStruct{ "Kanye", "Creative Genius", 43, 4039204, 2042185, 4, 45,
							   "Good Music", 430, 430, 200, 180, 9999, 3, 4, 9, 9, 4 , false },
			m_DetailsStruct{ 40, 40, 10, 10, 20, 20, 30, 30, 25, 13 }
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->createEvent(StatsUpdate);
		}

		void CharacterStats::updateStats()
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->execute(StatsUpdate);
		}

		Damage CharacterStats::getDamage() const
		{
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