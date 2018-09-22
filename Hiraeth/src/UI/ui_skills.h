#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/sprited_renderable.h"
#include "UI/ui_basic/ui_tabs.h"
#include "skills/skill_icon.h"

namespace hiraeth {
	namespace ui {

#define TAB_BEG 0
#define TAB_END 5

		class UiSkills : public UiWindow
		{
		private:
			UiTabs<skills::SkillIcon> * m_Tabs;

		public:
			UiSkills(maths::vec2 pos, input::Controls control_key);
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override {}
			void fillGroup();
			//void add_skill(skills::SkillIcon* skill, unsigned int tab);
			void add_skill(SRL::SkillData skill_data, unsigned tab)
			{
				UiTab<skills::SkillIcon> * rel_tab = m_Tabs->getTabByIndex(tab);
				skills::SkillIcon * skill = new skills::SkillIcon{ skill_data, {11, 172 - float(35* rel_tab->m_TabContent->m_Renderables.size())} };
				rel_tab->add_data(skill);
			}
		};

	}
}
