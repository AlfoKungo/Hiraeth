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
			ST_MaxExp, ST_Exp, // HonorLvl, HonorExp,
			ST_Str, ST_Dex, ST_Int, ST_Wit, ST_Med,
			ST_Guild, // pointer to guild index maybe?
			ST_MaxHp, ST_Hp, ST_MaxMp, ST_MaxMpByLvl, ST_Mp,
			ST_HpReg, ST_MpReg,
			//Fame,
			//AbilityPoints,
			//Attack, CritRate,
			//WeaponDef, MagicDef,
			//WeaponAcc, MagicAcc,
			//WeaponAvd, MagicAvd,
			ST_Speed, ST_Jump,
			ST_CritChance, ST_CritDmg,
		};
		enum UpdBaseTypes
		{
			UpdHealth, UpdMHealth,
			UpdMana, UpdMMana,

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
			unsigned int Str, Dex, Int, Wit, Med;
			//bool is_changed;
		};
		struct BaseStats
		{
			unsigned int MaxHp{}, MaxMp{};
			float HpReg{}, MpReg{};
			unsigned int Str{}, Dex{}, Int{}, Wit{}, Med{};
			unsigned int Attack{}, CritRate{}, CritDmg{};
			unsigned int Speed{}, Jump{};
		};
		struct AddOnStats
		{
			unsigned int MaxHp{}, MaxMp{};
			float HpReg{}, MpReg{};
			unsigned int Str{}, Dex{}, Int{}, Wit{}, Med{};
			unsigned int Attack{}, CritRate{}, CritDmg{};
			unsigned int Speed{}, Jump{};
		};
		struct DetailsStruct
		{
			unsigned int Attack, CritRate, CritDmg;
			unsigned int WeaponDef, MagicDef;
			unsigned int WeaponAcc, MagicAcc;
			unsigned int WeaponAvd, MagicAvd;
			float Speed, Jump;
		};
		struct IncStatStruct
		{
			StatsType stat_type;
			std::string val_func;
			unsigned int skill_lvl;
		};
		struct SimpleBonusStruct
		{
			//unsigned int skill_id;
			SRL::SkillPropertiesMap props;
			unsigned int skill_lvl{};
		};
		struct SimpleBonusStructB
		{
			//unsigned int skill_id;
			StatsType stat_type;
			unsigned int val{};
		};
		struct ComplexBonusStruct
		{
			float multi{};
			//StatsType stat_type;
			UpdBaseTypes upd_base_type{};
		};
		class CharacterStats : public Stats
		{
		private:
			StatsStruct m_StatsStruct;
			DetailsStruct m_DetailsStruct;
			BaseStats m_BaseStats{};
			AddOnStats m_AddOnStats;
			//std::vector<stats::TimedStat> m_TimedStats;
			std::vector<stats::TimedStat> m_TimedItemStats;
			//std::map<unsigned int, std::vector<IncStatStruct>> m_IncStatsMap;
			std::map<unsigned int, std::vector<stats::TimedStat>> m_TimedSkillStats;
			std::map<unsigned int, std::map<SRL::SkillDataType, int>> m_AttackModifiers;
			std::map<unsigned int, std::map<StatsType, unsigned int>> m_StatsIncBySimple;
			std::map<unsigned int, std::map<StatsType, unsigned int>> m_StatsIncByComplex;
			std::map<unsigned int, std::map<StatsType, ComplexBonusStruct>> m_ComplexBonuses;
			std::map<StatsType, unsigned int&> m_AddOnMap;
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

				for (const auto& [skill_id, comp_map] : m_ComplexBonuses)
				{
					removeComplexBonus(skill_id);
					for (const auto&[stat_type, comp] : comp_map)
					{
						addComplexBonus(skill_id, stat_type, comp);
					}
				}
				calculateStats();

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
			std::map<unsigned int, std::map<SRL::SkillDataType, int>>& getAttackModifiers() { return m_AttackModifiers; }

			bool activateUseItem(SRL::ItemPropertiesMap * item_properties);
			float getSpeed() const override;
			float getJump() const override;
			bool consumeMana(unsigned mp_consume) override;
			void recoverHp(unsigned heal_amount) override;
			void recoverMp(unsigned heal_amount);
			unsigned int getHpPercent() const;

			unsigned int getMHpPercent() const;

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

			void addAttackModifier(unsigned int skill_id, SRL::SkillDataType skill_data_type, 
				std::string val_func, unsigned int skill_lvl)
			{
				m_AttackModifiers[skill_id][skill_data_type] = getValueFromString(val_func, skill_lvl);
			}
			
			void updatePassiveSkill(unsigned int skill_id, unsigned int skill_lvl, const SRL::SkillPropertiesMap& skill_properties)
			{
				removeSimpleBonus(skill_id);
				removeComplexBonus(skill_id);
				for (const auto&[id, val] : (skill_properties))
				{
					switch (id)
					{
					case SRL::SkillDataType::inc_max_hp:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_MaxHp);
						break;
					case SRL::SkillDataType::inc_max_hp_per_lvl:
						addSimpleBonus(getInt(val, skill_lvl) * m_StatsStruct.Level, skill_id, ST_MaxHp);
						break;
					case SRL::SkillDataType::inc_max_mp:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_MaxMp);
						//increaseMaxMP(val);
						break;
					case SRL::SkillDataType::inc_max_mp_per_lvl:
						//addPassiveSkillValue(skill_id, ST_MaxMpByLvl,
						//	std::get<std::string>(val), skill_lvl);
						//incStatB(getInt(val, skill_lvl) * m_StatsStruct.Level, skill_id, m_AddOnStats.MaxMp, ST_MaxMp);
						addSimpleBonus(getInt(val, skill_lvl) * m_StatsStruct.Level, skill_id, ST_MaxMp);
						//increaseMaxMpPerLvl(val);
						break;
					case SRL::SkillDataType::inc_crit_chance:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_CritChance);
						break;
					case SRL::SkillDataType::lifesteal:
						//increaseCrit(val);
						addAttackModifier(skill_id, id,
							std::get<std::string>(val), skill_lvl);
						break;
					case SRL::inc_str:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_Str);
						break;
					case SRL::inc_dex:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_Dex);
						break;
					case SRL::inc_int:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_Int);
						break;
					case SRL::inc_wit:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_Wit);
						break;
					case SRL::inc_med:
						addSimpleBonus(getInt(val, skill_lvl), skill_id, ST_Med);
						break;
					//case SRL::inc_str_by_perc:
					//	//incStat(int(float(getInt(val, skill_lvl)) / 100.0f * m_BaseStats.Str), m_AddOnStats.Str, m_StatsIncBySkill[skill_id][ST_Str]);
					//	//incStatB(int(float(getInt(val, skill_lvl)) / 100.0f * m_BaseStats.Str), skill_id, m_AddOnStats.Str, ST_Str);
					//	m_ComplexBonuses[skill_id][ST_Str] = ComplexBonusStruct{ float(getInt(val, skill_lvl)) / 100.0f, UpdMHealth};
					//	break;
					case SRL::spd_by_mhealth:
						m_ComplexBonuses[skill_id][ST_Speed] = ComplexBonusStruct{ float(getInt(val, skill_lvl)), UpdMHealth};
						break;
					case SRL::jmp_by_mhealth:
						m_ComplexBonuses[skill_id][ST_Jump] = ComplexBonusStruct{ float(getInt(val, skill_lvl)), UpdMHealth};
						break;
					default:
						break;
					}
				}
				//recalculateEffectsFromSkills();
				calculateStats();
			}

			unsigned int getValueBaseOn(UpdBaseTypes upd_base, float multi)
			{
				switch (upd_base)
				{
				case UpdHealth:
					return (20 * float(m_StatsStruct.Hp) / float(m_StatsStruct.MaxHp)) * multi;
					break;
				case UpdMHealth:
					return (20 * (1.0f - float(m_StatsStruct.Hp) / float(m_StatsStruct.MaxHp))) * multi;
					break;
				case UpdMana:
					return (20 * float(m_StatsStruct.Mp) / float(m_StatsStruct.MaxMp)) * multi;
					break;
				case UpdMMana:
					return (20 * (1.0f - float(m_StatsStruct.Hp) / float(m_StatsStruct.MaxHp))) * multi;
					break;
				default:
					return 0;
				}
			}
			void removeSimpleBonus(unsigned int skill_id)
			{
				for (const auto& [id, val] : m_StatsIncBySimple[skill_id])
				{
					incAddOn(id, -val);
				}
				m_StatsIncBySimple.erase(skill_id);
			}
			void addSimpleBonus(int val, unsigned int skill_id, StatsType stat_type)
			{
				incAddOn(stat_type, val);
				m_StatsIncBySimple[skill_id][stat_type] = val;
			}
			void removeComplexBonus(unsigned int skill_id)
			{
				for (const auto& [id, val] : m_StatsIncByComplex[skill_id])
				{
					incAddOn(id, -val);
				}
				m_StatsIncByComplex.erase(skill_id);
			}
			void addComplexBonus(unsigned int skill_id, StatsType stat_type, ComplexBonusStruct cbs)
			{
				auto val = getValueBaseOn(cbs.upd_base_type, cbs.multi);
				incAddOn(stat_type, val);
				m_StatsIncByComplex[skill_id][stat_type] = val;
			}
			void calculateStats()
			{
				m_StatsStruct.MaxHp = m_BaseStats.MaxHp + m_AddOnStats.MaxHp;
				m_StatsStruct.MaxMp = m_BaseStats.MaxMp + m_AddOnStats.MaxMp;
				m_StatsStruct.HpReg = m_BaseStats.HpReg + m_AddOnStats.HpReg;
				m_StatsStruct.MpReg = m_BaseStats.MpReg + m_AddOnStats.MpReg;
				m_StatsStruct.Str = m_BaseStats.Str + m_AddOnStats.Str;
				m_StatsStruct.Dex = m_BaseStats.Dex + m_AddOnStats.Dex;
				m_StatsStruct.Int = m_BaseStats.Int + m_AddOnStats.Int;
				m_StatsStruct.Wit = m_BaseStats.Wit + m_AddOnStats.Wit;
				m_StatsStruct.Med = m_BaseStats.Med + m_AddOnStats.Med;
				m_DetailsStruct.Attack = m_BaseStats.Attack + m_AddOnStats.Attack;
				m_DetailsStruct.CritRate = m_BaseStats.CritRate + m_AddOnStats.CritRate;
				m_DetailsStruct.CritDmg = m_BaseStats.CritDmg + m_AddOnStats.CritDmg;
				m_DetailsStruct.Speed = m_BaseStats.Speed + m_AddOnStats.Speed;
			}
			void incAddOn(StatsType stat_type, int value)
			{
				m_AddOnMap.at(stat_type) += value;
			}
		};

	}
}