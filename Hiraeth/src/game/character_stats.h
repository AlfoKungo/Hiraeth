#pragma once

#include "stats.h"
#include <vector>
//#include "basic/event_handler.h"
#include "basic/EventManager.h"
#include "srl/item_data.h"
#include "basic/a_timer.h"
#include <tuple>
#include "stats/timed_stat.h"

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
			std::vector<stats::TimedStat> m_TimedStats;
			//std::vector<std::unique_ptr< TimedStat>> m_TimedStats;
		public:
			CharacterStats();


			void updateStats();
			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
			inline StatsStruct* getStatsStruct_() { return &m_StatsStruct; }
			inline DetailsStruct* getDetailsStruct_() { return &m_DetailsStruct; }

			bool activate_use_item(SRL::ItemPropertiesMap * item_properties);
			float getSpeed() const override;
			float getJump() const override;
			bool consumeMana(unsigned mp_consume) override;
			void recoverHp(unsigned heal_amount) override;
			void recoverMp(unsigned heal_amount);

			void setTimedStat(float& data_member, SRL::TimedValue timed_value)
			{
				m_TimedStats.emplace_back(stats::TimedStat{ &data_member, timed_value });
			}

			void update()
			{
				m_TimedStats.erase(
					std::remove_if(begin(m_TimedStats), end(m_TimedStats), 
					[](stats::TimedStat ts) {return ts.checkAndCancel(); }), end(m_TimedStats));
			}
		};

	}
}
