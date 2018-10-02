#pragma once

#include "stats.h"
#include <vector>
//#include "basic/event_handler.h"
#include "basic/EventManager.h"
#include "srl/item_data.h"
#include "basic/a_timer.h"
#include <tuple>
#include "stats/timed_stat.h"
#include "srl/skill_data.h"
#include "item/item.h"
#include "item/equip_item.h"

namespace hiraeth {
	namespace game {

		struct StatsStruct
		{
			std::string Name, Job;
			unsigned int Level;
			unsigned int MaxExp, Exp, HonorLvl, HonorExp;
			std::string Guild;
			unsigned int MaxHp, Hp, MaxMp, Mp;
			unsigned int Fame;
			unsigned int AbilityPoints;
			unsigned int Str, Int, Luk, Dex;
			bool is_changed;
		};
		struct DetailsStruct
		{
			unsigned int Attack, CritRate;
			unsigned int WeaponDef, MagicDef;
			unsigned int WeaponAcc, MagicAcc;
			unsigned int WeaponAvd, MagicAvd;
			float Speed, Jump;
		};

		class CharacterStats : public Stats
		{
		private:
			StatsStruct m_StatsStruct;
			DetailsStruct m_DetailsStruct;
			//std::vector<stats::TimedStat> m_TimedStats;
			std::vector<stats::TimedStat> m_TimedItemStats;
			std::map<unsigned int, std::vector<stats::TimedStat>> m_TimedSkillStats;
		public:
			CharacterStats();


			void updateStats();
			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
			inline StatsStruct* getStatsStruct_() { return &m_StatsStruct; }
			inline DetailsStruct* getDetailsStruct_() { return &m_DetailsStruct; }

			bool activateUseItem(SRL::ItemPropertiesMap * item_properties);
			float getSpeed() const override;
			float getJump() const override;
			bool consumeMana(unsigned mp_consume) override;
			void recoverHp(unsigned heal_amount) override;
			void recoverMp(unsigned heal_amount);

			void setTimedStat(float& data_member, SRL::TimedValue timed_value)
			{
				m_TimedItemStats.emplace_back(stats::TimedStat{ &data_member, timed_value });
			}
			void setTimedStat(unsigned int skill_index, float& data_member, SRL::TimedValue timed_value)
			{
				if (m_TimedSkillStats.find(skill_index) != m_TimedSkillStats.end())
					m_TimedSkillStats[skill_index].clear();
				m_TimedSkillStats[skill_index].emplace_back( &data_member, timed_value );
			}

			void activateSkill(unsigned int skill_index, SRL::SkillPropertiesMap * skill_properties)
			{
				for (SRL::SkillPropertiesMapPair element : (*skill_properties))
				{
					switch (element.first)
					{
					//case SRL::SkillDataType::dmg:
					//	causeDamage(Damage{ unsigned int(std::get<int>(element.second)) });
					//	break;
					case SRL::SkillDataType::heal:
						recoverHp(std::get<int>(element.second));
						break;
					case SRL::SkillDataType::speed:
						setTimedStat(skill_index, m_DetailsStruct.Speed, std::get<2>(element.second));
						break;
					//case SRL::SkillDataType::dmgS:
					//	causeDamage(Damage{ 25, 25 });
					//	break;
					default:
						break;
					}
				}
			}
			void update()
			{
				m_TimedItemStats.erase(
					std::remove_if(begin(m_TimedItemStats), end(m_TimedItemStats), 
					[](stats::TimedStat ts) {return ts.checkTimer(); }), end(m_TimedItemStats));
				for (auto it = m_TimedSkillStats.begin(); it != m_TimedSkillStats.end() /* not hoisted */; /* no increment */)
				{
					(it)->second.erase(
						std::remove_if(begin((it)->second), end((it)->second),
							[](const stats::TimedStat& ts) {return ts.checkTimer(); }), end((it)->second));
					if (it->second.empty())
						it = m_TimedSkillStats.erase(it);  
					else
						++it;
				}
			}

			void wearItem(item::EquipItem * item)
			{
				
			}
			void unwearItem(item::EquipItem * item)
			{
				
			}
		};

	}
}
