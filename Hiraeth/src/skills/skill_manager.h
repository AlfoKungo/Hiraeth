#pragma once
#include "basic/updatable.h"
#include "skill_icon.h"
#include "top_right_icon.h"
#include "graphics/percented_sprite.h"
#include "UI/ui_skills.h"
#include "srl/job_data.h"

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

			void setJobAndLoadSkills(unsigned int job_id, const std::vector<network::SkillAlloc>& skills_alloc)
				//void setJob(unsigned int job_id, std::map<unsigned int, unsigned int> stats_alloc)
			{
				std::vector<unsigned int> ZeroJobSkillsIndices = SRL::JobInfo.zero_jobs;
				for (const auto& index : ZeroJobSkillsIndices)
					add_skill(index, 0);
				loadJobData(job_id);
				loadSkillPoints(skills_alloc);
			}

			void updateJob(unsigned int new_job_id)
			{
				eraseSkills();
				loadJobData(new_job_id);

			}
			
			void loadJobData(unsigned int job_id)
			{
				//const SRL::JobsTypes job_type{ static_cast<SRL::JobsTypes>(job_id) };
				unsigned int first_job_id = div(job_id, 1000).quot;
				const SRL::JobsTypes first_job_type{ static_cast<SRL::JobsTypes>(first_job_id) };
				if (first_job_id != 0)
				{
					std::vector<unsigned int> FirstJobSkillsIndices = SRL::JobInfo.first_jobs.at(first_job_type).first_job_skills;
					for (const auto& index : FirstJobSkillsIndices)
						add_skill(index, 1);
				}
				unsigned int second_job_id = div(div(job_id, 1000).rem, 100).quot;
				if (second_job_id != 0)
				{
					std::vector<unsigned int> SecondJobSkillsIndices = SRL::JobInfo.first_jobs.at(first_job_type).second_job_skills.at(second_job_id);
					for (const auto& index : SecondJobSkillsIndices)
						add_skill(index, 1);
				}
			}
			void loadSkillPoints(const std::vector<network::SkillAlloc>& skills_alloc)
			{
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
			
			void eraseSkills()
			{

			}

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
