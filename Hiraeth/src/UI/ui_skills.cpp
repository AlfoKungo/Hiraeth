#include "ui_skills.h"
#include "basic\EventManager.h"

namespace hiraeth {
	namespace ui {

		UiSkills::UiSkills(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 174, 299), control_key),
			m_Tab(0)
		{
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd.png")));
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(7, 28), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd3.png")));

			// add tabs graphics - disabled and enabled
			for (int tab_index = TAB_BEG; tab_index <= TAB_END; ++tab_index)
			{
				m_SkillTabs.insert(std::pair<unsigned int, std::unique_ptr<graphics::Group>>(tab_index, new graphics::Group()));
				m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(9 + 31 * tab_index, 252), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.Tab.disabled." + std::to_string(tab_index) + ".png")));
				m_SkillTabs[tab_index]->add(new graphics::Sprite(maths::vec2(9 + 31 * tab_index, 252), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.Tab.enabled." + std::to_string(tab_index) + ".png")));
			}
			fillGroup();
		}

		void UiSkills::fillGroup()
		{
			m_ForegroundGroup.clear();
			m_ForegroundGroup.add(m_SkillTabs[m_Tab].get());
		}

		void UiSkills::mouse_clicked(maths::vec2 mousePos)
		{
			//if (mousePos.y < 270 && mousePos.y > 252)
			for (auto const & tab : m_SkillTabs)
				if (tab.second.get()->m_Renderables[0]->getBounds().Contains(mousePos))
				{
					//m_Tab = unsigned int((mousePos.x - 11) / 30);
					m_Tab = tab.first;
					fillGroup();
				}
		}
	}
}