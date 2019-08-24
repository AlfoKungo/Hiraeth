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
#include "utils/calculator.hpp"

namespace hiraeth {
	namespace game {

		const float BASE_HP_REG = 0.4f, BASE_MP_REG = 1.0f;
		enum StatsType
		{
			//Level,
			MaxExp, Exp, // HonorLvl, HonorExp,
			Guild, // pointer to guild index maybe?
			MaxHp, Hp, MaxMp, MaxMpByLvl, Mp,
			//Fame,
			//AbilityPoints,
			//Str, Int, Luk, Dex,
			//Attack, CritRate,
			//WeaponDef, MagicDef,
			//WeaponAcc, MagicAcc,
			//WeaponAvd, MagicAvd,
		 // Speed, Jump,
		  CritChance, CritDmg,
		};
		struct StatsStruct
		{
			std::string Name, Job;
			unsigned int Level;
			unsigned int MaxExp, Exp, HonorLvl, HonorExp;
			std::string Guild;
			unsigned int MaxHp, Hp, MaxMp, Mp;
			unsigned int bMaxHp, bMaxMp;
			float HpReg, MpReg;
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
		struct IncStatStruct
		{
			StatsType stat_type;
			//float val_inc;
			std::string val_func;
			unsigned int skill_lvl;
		};

		class CharacterStats : public Stats
		{
		private:
			StatsStruct m_StatsStruct;
			DetailsStruct m_DetailsStruct;
			//std::vector<stats::TimedStat> m_TimedStats;
			std::vector<stats::TimedStat> m_TimedItemStats;
			std::map<unsigned int, std::vector<IncStatStruct>> m_IncStatsMap;
			std::map<unsigned int, std::vector<stats::TimedStat>> m_TimedSkillStats;
			std::map<unsigned int, std::vector<std::pair<SRL::SkillDataType, int>>> m_AttackModifiers;
			float m_HpAccum{0.0f}, m_HpH{0.0f}, m_MpAccum{0.0f}, m_MpH{0.0f};
		public:
			CharacterStats();
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
				m_MpAccum += (StaticTimer::timer.elapsed() - m_MpH) * m_StatsStruct.MpReg;
				m_MpH = StaticTimer::timer.elapsed();
				m_StatsStruct.Mp = std::min<unsigned int>(m_StatsStruct.Mp + unsigned int(floor(m_MpAccum)), m_StatsStruct.MaxMp);
				m_MpAccum -= floor(m_MpAccum);
				m_HpAccum += (StaticTimer::timer.elapsed() - m_HpH) * m_StatsStruct.HpReg;
				m_HpH = StaticTimer::timer.elapsed();
				m_StatsStruct.Hp = std::min<unsigned int>(m_StatsStruct.Hp + unsigned int(floor(m_HpAccum)), m_StatsStruct.MaxHp);
				m_HpAccum -= floor(m_HpAccum);
				updateStats();
			}


			void updateStats();
			Damage getDamage() const override;
			void causeDamage(Damage damage) override;
			inline StatsStruct* getStatsStruct_() { return &m_StatsStruct; }
			inline DetailsStruct* getDetailsStruct_() { return &m_DetailsStruct; }
			std::map<unsigned int, std::vector<std::pair<SRL::SkillDataType, int>>>& getAttackModifiers() { return m_AttackModifiers; }

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

			void wearItem(item::EquipItem * item)
			{
				applyItemStats(item, 1);
			}

			void unwearItem(item::EquipItem * item)
			{
				applyItemStats(item, -1);
			}

			void applyItemStats(item::EquipItem * item, int multi)
			{
				for (auto& element : (item->getProperties()))
				{
					switch (element.first)
					{
					case SRL::ReqLvl: break;
					case SRL::ReqStr: break;
					case SRL::ReqDex: break;
					case SRL::ReqLuk: break;
					case SRL::ReqInt: break;
					case SRL::AttackPower:
						m_DetailsStruct.Attack += std::get<int>(element.second) * multi;
						break;
					case SRL::StrInc:
						m_StatsStruct.Str += std::get<int>(element.second) * multi;
						break;
					case SRL::DexInc:
						m_StatsStruct.Dex += std::get<int>(element.second) * multi;
						break;
					case SRL::LukInc:
						m_StatsStruct.Luk += std::get<int>(element.second) * multi;
						break;
					case SRL::IntInc:
						m_StatsStruct.Int += std::get<0>(element.second) * multi;
						break;
					case SRL::MaxHpInc: break;
					case SRL::MaxMpInc: break;
					default:
						break;
					}
				}
				EventManager *m_EventManager = EventManager::Instance();
				m_EventManager->execute(StatsUpdate);
			}
			int getValueFromString(std::string str, unsigned int val)
			{
				auto pos = str.find('x');
				if (pos != std::string::npos)
					str.replace(pos, 1, std::to_string(val));
				return calculator::eval(str);
			}
			int getInt(SRL::SkillPropertiesVar val, unsigned int skill_lvl)
			{

				if (std::holds_alternative<int>(val))
					return std::get<int>(val);
				std::string con_string = std::get<std::string>(val);
				return getValueFromString(con_string, skill_lvl);
			}
			void removeEffectsFromSkill(unsigned int skill_id)
			{
				m_IncStatsMap.erase(skill_id);
				m_AttackModifiers.erase(skill_id);
			}
			void recalculateEffectsFromSkills()
			{
				returnAllToBaseStats();
				reCalculateAllStats();
				EventManager *m_EventManager = EventManager::Instance();
				m_EventManager->execute(StatsUpdate);
			}
			void applySkillStats(unsigned int skill_id, StatsType stat_type, 
				std::string val_func, unsigned int skill_lvl)
			{
				float amount = float(getValueFromString(val_func, skill_lvl));
				//for (auto& element : (item->getProperties()))
				//{
					switch (stat_type)
					{
					//case Str: break;
					//case Dex: break;
					//case Luk: break;
					//case Int: break;
					case MaxMp:
						m_StatsStruct.MaxMp *= (1.0f + (amount/ 100.0f));
						break;
					case MaxMpByLvl:
						m_StatsStruct.MaxMp += amount * m_StatsStruct.Level;
						break;
					case CritChance:
						m_DetailsStruct.CritRate += amount;
						break;
					default:
						break;
					}
				//}
			}
			void returnAllToBaseStats()
			{
				m_StatsStruct.MaxHp = m_StatsStruct.bMaxHp;
				m_StatsStruct.MaxMp = m_StatsStruct.bMaxMp;
				m_DetailsStruct.CritRate = 40;
			}
			void addPassiveSkillValue(unsigned int skill_id, StatsType stat_type, 
				std::string val_func, unsigned int skill_lvl)
			{
				m_IncStatsMap[skill_id].push_back(IncStatStruct{ stat_type, val_func, skill_lvl});
			}
			void addAttackModifier(unsigned int skill_id, SRL::SkillDataType skill_data_type, 
				std::string val_func, unsigned int skill_lvl)
			{
				m_AttackModifiers[skill_id].push_back(std::make_pair( skill_data_type, getValueFromString(val_func, skill_lvl)));
			}
			void reCalculateAllStats()
			{
				for (const auto& [key, vals] : m_IncStatsMap)
				{
					for (const auto& inc_val : vals)
					{
						applySkillStats(key, inc_val.stat_type, inc_val.val_func, inc_val.skill_lvl);
					}
				}
			}
			void updatePassiveSkill(unsigned int skill_id, unsigned int skill_lvl, SRL::SkillPropertiesMap* skill_properties)
			{
				removeEffectsFromSkill(skill_id);
				for (const auto&[id, val] : (*skill_properties))
				{
					switch (id)
					{
					case SRL::SkillDataType::immp:
						addPassiveSkillValue(skill_id, game::MaxMp,
							std::get<std::string>(val), skill_lvl);
						//increaseMaxMP(val);
						break;
					case SRL::SkillDataType::mmppl:
						addPassiveSkillValue(skill_id, game::MaxMpByLvl,
							std::get<std::string>(val), skill_lvl);
						//increaseMaxMpPerLvl(val);
						break;
					case SRL::SkillDataType::crit:
						//increaseCrit(val);
						addPassiveSkillValue(skill_id, game::CritChance,
							std::get<std::string>(val), skill_lvl);
						break;
					case SRL::SkillDataType::lifesteal:
						//increaseCrit(val);
						addAttackModifier(skill_id, id,
							std::get<std::string>(val), skill_lvl);
						break;
					default:;
					}
				}
				recalculateEffectsFromSkills();
			}
		};

	}
}