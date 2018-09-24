#include "ui_skills.h"
#include "basic\EventManager.h"
#include "srl/monster_data.h"

namespace hiraeth {
	namespace ui {

		UiSkills::UiSkills(maths::vec2 pos, UiKey control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 174, 299), control_key),
			m_Tabs(new UiTabs<skills::SkillIcon>())
		{
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(7, 28), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd3.png")));

			for (int tab_index = TAB_BEG; tab_index < TAB_END; ++tab_index)
			{
				m_Tabs->add_tab(tab_index, maths::vec2(9 + 31 * tab_index, 252), "Skills", "main", m_BackgroundGroup);
			}
			m_BackgroundGroup->add(m_Tabs);

		}

		

		void UiSkills::fillGroup()
		{
		}

		void UiSkills::mouse_left_clicked(maths::vec2 mousePos)
		{
			if (mousePos.y < 270 && mousePos.y > 252)
			{
				m_Tabs->mouse_left_clicked(mousePos);
			}
			else
			{

				std::vector<std::unique_ptr<skills::SkillIcon>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
				auto result_skill = std::find_if(std::begin(*tab_rends),
					std::end(*tab_rends), [&](auto const& skill)
				{ 
				return skill->getSpUpButtonBounds().Contains(mousePos); });
				if (result_skill != std::end(*tab_rends))
				{
					(*result_skill)->inc_level();
					return;
				}
			}
		}

		void UiSkills::mouse_moved(float mx, float my, maths::vec2 mousePos)
		{

				std::vector<std::unique_ptr<skills::SkillIcon>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
				for (auto & skill_icon : *tab_rends)
					skill_icon->updateIconState(false);
				auto result_skill = std::find_if(std::begin(*tab_rends),
					std::end(*tab_rends), [&](auto const& skill_icon)
				{ return skill_icon->getBounds().Contains(mousePos); });
				if (result_skill != std::end(*tab_rends))
				{
					(*result_skill)->updateIconState(true);
					return;
				}
		}
	}
}
