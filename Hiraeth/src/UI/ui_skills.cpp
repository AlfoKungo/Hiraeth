#include "ui_skills.h"
#include "basic/EventManager.h"
#include "srl/monster_data.h"
//#include "network/client_handler.h"

namespace hiraeth {
	namespace ui {

		UiSkills::UiSkills(maths::vec2 pos, UiKey control_key, UiKeyConfig * ui_key_config)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 174, 299), control_key),
			m_SkillPtsLabel("arial", 13, m_SkillPts, maths::vec2{ 144, 31 }, 0xff000000, graphics::Label::Alignment::LEFT),
			m_Tabs(new UiTabs<skills::SkillIcon>()),
		m_UiKeyConfig(ui_key_config)
		{
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(7, 28), graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.backgrnd3.png")));

			for (int tab_index = TAB_BEG; tab_index < TAB_END; ++tab_index)
			{
				m_Tabs->add_tab(tab_index, maths::vec2(9 + 31 * tab_index, 252), "Skills", "main", m_BackgroundGroup);
			}
			m_BackgroundGroup->add(m_Tabs);
			fillGroup();
		}

		

		void UiSkills::fillGroup()
		{
			m_ForegroundGroup->add_ref(&m_SkillPtsLabel);
		}

		void UiSkills::mouse_left_clicked(maths::vec2 mousePos)
		{
			if (mousePos.y < 270 && mousePos.y > 252)
				m_Tabs->mouse_left_clicked(mousePos);
			else
			{
				//std::vector<std::unique_ptr<skills::SkillIcon>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
				//auto result_skill = std::find_if(std::begin(*tab_rends),
				//	std::end(*tab_rends), [&](auto const& skill)
				//{ return skill->getSpUpButtonBounds().Contains(mousePos); });
				//if (result_skill != std::end(*tab_rends))
				//{
				//	m_UiKeyConfig->addIcon(GLFW_KEY_A, SkillUiIcon, (*result_skill).get()->get_skill_info()->name);
				//	//m_UiKeyConfig->addIcon(GLFW_KEY_A, (*result_skill)->, SkillUiIcon, (*result_skill).get()->get_skill_info()->name);
				//}
			}
		}

		void UiSkills::transferSkillToKeyConfig(input::KeyCode key_code, unsigned int key, unsigned int skill_index, input::KeyboardEvent * keyboard_event)
		{
			auto skill_to_assign = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables[skill_index];
			m_UiKeyConfig->addSkillIcon(key_code, key, (*skill_to_assign)->get_skill_info()->name, keyboard_event);
		}

		void UiSkills::mouse_right_clicked(maths::vec2 mousePos)
		{
			if (m_SkillPts > 0)
			{
				std::vector<std::unique_ptr<skills::SkillIcon>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
				auto result_skill = std::find_if(std::begin(*tab_rends),
					std::end(*tab_rends), [&](auto const& skill)
				{
					return skill->getSpUpButtonBounds().Contains(mousePos); });
				if (result_skill != std::end(*tab_rends))
				{
					auto changed_skill = (*result_skill)->getSkillId();
					EventManager *m_EventManager = EventManager::Instance();
					m_EventManager->execute<unsigned int>(SendIncreaseSkill, changed_skill);
					//m_ClientHandler->sendIncreaseSkill(changedSkill);
					if ((*result_skill)->inc_level())
					{
						m_SkillPts--;
						m_SkillPtsLabel.setText(m_SkillPts);
					}
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
