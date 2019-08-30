#pragma once
#include "basic/updatable.h"
#include "skill_icon.h"
#include "top_right_icon.h"
#include "graphics/percented_sprite.h"
#include "UI/ui_skills.h"

namespace hiraeth {
	namespace skills {

		class SkillManager : public Updatable
		{
			std::map<unsigned int, std::tuple<SRL::SkillInfo*, SRL::AnimationMap*>> m_SkillsData;
			std::map<unsigned int, unsigned int> m_SkillsAlloc;
			game::CharacterStats* m_CharacterStats;
		public:
			ui::UiSkills * m_UiSkills;
			graphics::Layer<TopRightIcon> m_Layer;
			SkillManager(ui::UiSkills * ui_skills, game::CharacterStats * character_stats)
				: m_CharacterStats(character_stats),
				m_UiSkills(ui_skills),
				m_Layer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), false)
			{
				//SRL::AllJobsData JobInfo{ {
				//{ SRL::Berserker, {{1,2,3}, {1,2,3}}},
				//{ SRL::CrusaderKnight, {{1,2,3}, {1,2,3}}},
				//{ SRL::Wizard, {{1,2,3}, {1,2,3}}},
				//{ SRL::Rogue, {{1,2,3}, {1,2,3}}},
				//{ SRL::Archer, {{1,2,3}, {1,2,3}}},
				//{ SRL::ForestFighter, {{1,2,3}, {1,2,3}}},
				//	} };

				////std::vector<unsigned int> SkillIndices = SRL::deserial<std::vector<unsigned int>>("serialized/jobs.data", 0);
				//std::vector<unsigned int> FirstJobSkillsIndices = JobInfo.jobs_type_to_data_map.at(SRL::Berserker).first_job_skills;
				//for (const auto& index : FirstJobSkillsIndices)
				//	add_skill(index - 1);

			}

			void update() override
			{
				m_Layer.update();
				size_t size_before{ m_Layer.m_Renderables.size() };
				m_Layer.m_Renderables.erase(
					std::remove_if(begin(m_Layer.m_Renderables), end(m_Layer.m_Renderables),
						[](graphics::Renderable* ts) {return ts->hasSpriteFinished(); }), end(m_Layer.m_Renderables));
				if (m_Layer.m_Renderables.size() != size_before)
					reconfigure_icons();
			}

			void draw() const
			{
				m_Layer.render();
			}

			void add_skill(unsigned int index, unsigned int job_index)
			{
				//const SRL::SkillData skill_data = SRL::deserial<SRL::SkillData>("skills", index);
				//m_SkillsData[index] = m_UiSkills->add_skill(index, skill_data, 0);
				m_SkillsData[index] = m_UiSkills->add_skill(index, job_index);
			}

			SRL::SkillInfo* get_skill_info(unsigned int skill_index)
			{
				if (m_SkillsData.find(skill_index) != m_SkillsData.end())
					return std::get<0>(m_SkillsData[skill_index]);
				return nullptr;
			}

			SRL::AnimationMap* getAnimationData(unsigned int skill_index)
			{
				if (m_SkillsData.find(skill_index) != m_SkillsData.end())
					return std::get<SRL::AnimationMap*>(m_SkillsData[skill_index]);
				return nullptr;
			}

			std::vector<unsigned int> get_available_active_skills()
			{
				std::vector<unsigned int> vints;
				vints.reserve(m_SkillsData.size());
				for (auto const& imap : m_SkillsData)
					if (std::get<0>(imap.second)->skill_type < SRL::passive_s)
						vints.push_back(imap.first);
				return vints;
			}
			std::vector<unsigned int> get_effect_skills()
			{
				std::vector<unsigned int> vints;
				vints.reserve(m_SkillsData.size());
				for (auto const& imap : m_SkillsData)
					if (std::get<0>(imap.second)->skill_type == SRL::passive_effect)
						vints.push_back(imap.first);
				return vints;
			}
			unsigned int getSkillAlloc(unsigned int skill_id)
			{
				//return m_UiSkills->getSkillAlloc(skill_id);
				return m_SkillsAlloc[skill_id];
			}
			void increasSkill(unsigned int skill_id)
			{
				m_SkillsAlloc[skill_id] += 1;
				m_SkillsAlloc[666] -= 1;
				m_UiSkills->setSkillPoints(skill_id, m_SkillsAlloc[skill_id]);
				m_UiSkills->setSkillPoints(666, m_SkillsAlloc[666]);
				updateSkillEffectIfPassive(skill_id);
			}
			void updateSkillEffectIfPassive( unsigned int skill_id)
			{
					auto skill_info = get_skill_info(skill_id);
					if (skill_info != nullptr)
						if (skill_info->skill_type >= SRL::passive_s)
							//updatePassiveSkill(alloc.skill_id, m_SkillsAlloc[alloc.skill_id], &skill_info->skill_properties);
							m_CharacterStats->updatePassiveSkill(skill_id, m_SkillsAlloc[skill_id], skill_info->skill_properties);
			}

			void setJobAndLoadSkills(unsigned int job_id, std::vector<network::SkillAlloc> skills_alloc)
			//void setJob(unsigned int job_id, std::map<unsigned int, unsigned int> stats_alloc)
			{
				SRL::AllJobsData JobInfo{ {
				{ SRL::Novice, {{0,1,2}, {}}},
				{ SRL::Berserker, {{0,1,2}, {5,6,7}}},
				{ SRL::CrusaderKnight, {{0,1,2}, {10,11}}},
				{ SRL::Wizard, {{0,1,2}, {3,4}}},
				{ SRL::Rogue, {{0,1,2}, {3,4}}},
				{ SRL::Archer, {{0,1,2}, {3,4}}},
				{ SRL::ForestFighter, {{0,1,2}, {1,2,3}}},
					} };

				//std::vector<unsigned int> SkillIndices = SRL::deserial<std::vector<unsigned int>>("serialized/jobs.data", 0);
				const SRL::JobsTypes job_type{ static_cast<SRL::JobsTypes>(job_id) };
				std::vector<unsigned int> BasicJobSkillsIndices = JobInfo.jobs_type_to_data_map.at(job_type).first_job_skills;
				for (const auto& index : BasicJobSkillsIndices)
					add_skill(index, 0 );
				std::vector<unsigned int> FirstJobSkillsIndices = JobInfo.jobs_type_to_data_map.at(job_type).second_job_skills;
				for (const auto& index : FirstJobSkillsIndices)
					add_skill(index, 1);

				for (const auto& alloc : skills_alloc)
				{
					m_SkillsAlloc[alloc.skill_id] = alloc.pts_alloc;
					m_UiSkills->setSkillPoints(alloc.skill_id, alloc.pts_alloc);
					updateSkillEffectIfPassive(alloc.skill_id);
				}
			}

			//void updatePassiveSkill(unsigned int skill_id, unsigned int skill_lvl, SRL::SkillPropertiesMap* skill_properties)
			//{
			//	m_CharacterStats->removeEffectsFromSkill(skill_id);
			//	for (const auto& [id, val] : (*skill_properties))
			//	{
			//		switch (id)
			//		{
			//		case SRL::SkillDataType::immp:
			//			m_CharacterStats->addPassiveSkillValue(skill_id, game::MaxMp, 
			//				std::get<std::string>(val), skill_lvl);
			//			//increaseMaxMP(val);
			//			break;
			//		case SRL::SkillDataType::mmppl:
			//			m_CharacterStats->addPassiveSkillValue(skill_id, game::MaxMpByLvl,
			//				std::get<std::string>(val), skill_lvl);
			//			//increaseMaxMpPerLvl(val);
			//			break;
			//		case SRL::SkillDataType::crit:
			//			//increaseCrit(val);
			//			m_CharacterStats->addPassiveSkillValue(skill_id, game::CritChance, 
			//				std::get<std::string>(val), skill_lvl);
			//			break;
			//		default:;
			//		}
			//	}
			//	m_CharacterStats->recalculateEffectsFromSkills();
			//}

			void add_icon(const std::string& name, unsigned int skill_index, float duration)
			{
				std::vector<TopRightIcon*>* rends = &m_Layer.m_Renderables;
				rends->erase(std::remove_if(rends->begin(), rends->end(), // remove icon if already exists
					[skill_index](const TopRightIcon* rend) {return rend->getSkillIndex() == skill_index; }), rends->end());

				SRL::AnimationData ad{ {},false };
				ad.frames_data.push_back({ {0,0,32,32},{16,0}, duration });
				m_Layer.add(new TopRightIcon{ skill_index, {float(768 - m_Layer.m_Renderables.size() * 32), 418 },
					graphics::TextureManager::Load(name), duration });
			}

			void reconfigure_icons()
			{
				for (int i = 0; i < m_Layer.m_Renderables.size(); ++i)
				{
					m_Layer.m_Renderables[i]->setPosition({ float(768 - i * 32), 418 });
				}
			}

		};
	}
}
