#pragma once

#include "stats.h"
#include <vector>
//#include "basic/event_handler.h"
#include "basic/EventManager.h"

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
			//std::vector<StatsUpdateEvent*> m_StatsUpdateListeners;
		public:
			CharacterStats();
			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
			inline StatsStruct* getStatsStruct_() { return &m_StatsStruct; }
			inline DetailsStruct* getDetailsStruct_() { return &m_DetailsStruct; }
			void add_stats(std::string stats_string)
			{
				std::string stat_type = stats_string.substr(0, 3);
				std::string stat_value = stats_string.substr(4);
				unsigned int value = atoi(stat_value.c_str())/4;
				if (stat_type == "spd")
					m_DetailsStruct.Speed += value;
				//else
				//	std::cout << "added nothing" << "\n";
			}
			//void registerToStatsUpdate(StatsUpdateEvent* listener) { m_StatsUpdateListeners.push_back(listener); }
		};

	}
}