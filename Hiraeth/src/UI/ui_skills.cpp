#include "ui_skills.h"
#include "basic\EventManager.h"

namespace hiraeth {
	namespace ui {

		UiSkills::UiSkills(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 174, 299), control_key),
			m_SkillTabs(&m_ForegroundGroup)
		{
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd.png")));
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(7, 28), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd3.png")));

			for (int tab_index = TAB_BEG; tab_index < TAB_END; ++tab_index)
			{
				m_SkillTabs.add_tab(tab_index, maths::vec2(9 + 31* tab_index, 252), "Skills", "main", &m_BackgroundGroup);
			}
		}

		void UiSkills::fillGroup()
		{
		}

		void UiSkills::mouse_clicked(maths::vec2 mousePos)
		{
			m_SkillTabs.mouse_clicked(mousePos);
		}
	}
}