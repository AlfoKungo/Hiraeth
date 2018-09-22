#pragma once
#include "basic/updatable.h"
#include "skill_icon.h"
#include "UI/ui_skills.h"

namespace hiraeth {
	namespace skills {

		class SkillManager: public Updatable
		{
			std::map<unsigned int, SRL::SkillInfo > m_SkillsInfo;
			ui::UiSkills * m_UiSkills;
		public:
			SkillManager(ui::UiSkills * ui_skills)
				: m_UiSkills(ui_skills)
			{
				add_skill(0);
				add_skill(1);
				add_skill(2);
				
			}

			void update() override{}

			void add_skill(unsigned int index)
			{
				SRL::SkillData skill_data = SRL::deserial<SRL::SkillData>("serialized/skills.data", index);
				m_UiSkills->add_skill(skill_data, 0);
				m_SkillsInfo[index] = skill_data.skill_info;
			}

			SRL::SkillInfo* get_skill(unsigned int skill_index)
			{
				if (m_SkillsInfo.find(skill_index) != m_SkillsInfo.end())
					return &m_SkillsInfo[skill_index];
				return nullptr;
			}
		};
	}
}
