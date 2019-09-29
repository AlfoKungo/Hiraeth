#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/sprited_renderable.h"
#include "UI/ui_basic/ui_tabs.h"
#include "skills/skill_icon.h"
#include "ui_keyconfig.h"
#include "srl/skill_data.h"
#include <cereal/types/memory.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>
#include "srl/deserial.h"
#include "srl/ImageLoad.h"
#include "basic/updatable.h"
#include "skills/skill_icon.h"
#include "UI/ui_skills.h"
#include "skills/top_right_icon.h"
#include "graphics/percented_sprite.h"

//namespace hiraeth {
//	namespace network {
//		class ClientHandler;
//	}
//}
namespace hiraeth {
	namespace ui {

#define TAB_BEG 0
#define TAB_END 5

		class UiSkills : public UiWindow
		{
		private:
			//UiTabs<skills::SkillIcon> * m_Tabs;
			//UiTabs<skills::SkillIcon> m_Tabs;
			UiTabs<UiTab<skills::SkillIcon>> m_Tabs;
			unsigned int m_SkillPts{6};
			graphics::Label m_SkillPtsLabel;
			UiKeyConfig * m_UiKeyConfig;
			//network::ClientHandler * m_ClientHandler;
		public:
			UiSkills(maths::vec2 pos, UiKey control_key, UiKeyConfig * ui_key_config);
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override;
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override;
			void fillGroup();
			//void setClientHandler(network::ClientHandler * client_handler) { m_ClientHandler = client_handler; }
			std::tuple<SRL::SkillInfo*, SRL::AnimationMap*> add_skill(unsigned int skill_id, unsigned tab)
			{
				const SRL::SkillData skill_data = SRL::deserial<SRL::SkillData>("skills", skill_id);
				UiTab<skills::SkillIcon> * rel_tab = m_Tabs.getTabByIndex(tab);
				skills::SkillIcon * skill = new skills::SkillIcon{ skill_id, {11, 172 - float(36 * rel_tab->m_TabContent->m_Renderables.size())} };
				rel_tab->add_data(skill);
				return std::make_tuple(skill->get_skill_info(), skill->get_animation_data());
			}
			void transferSkillToKeyConfig(input::KeyCode key_code, unsigned int key, unsigned int skill_index, input::KeyboardEvent * keyboard_event);
			unsigned int getSkillAlloc(unsigned int skill_id)
			{
				return (*m_Tabs.getTabByIndex(0)->m_TabContent->m_Renderables[skill_id]).getSkillLvl();
			}
			void setSkillPoints(unsigned int skill_id, unsigned int alloc)
			{
				if (skill_id == 666)
				{
					m_SkillPts = alloc;
					m_SkillPtsLabel.setText(m_SkillPts);
				}
				else
					for (int i = 0; i < 5; ++i)
					{
						for (const auto& skill_icon : m_Tabs.getTabByIndex(i)->m_TabContent->m_Renderables)
							if (skill_icon->getSkillId() == skill_id)
							{
								(*skill_icon).set_level(alloc);
								return;
							}
						//(*m_Tabs->getTabByIndex(0)->m_TabContent->m_Renderables[skill_id]).set_level(alloc);
					}
			}
			//std::unique_ptr<skills::SkillIcon>* getSkill(unsigned int skill_id)
			skills::SkillIcon* getSkillIcon(unsigned int skill_id)
			{
					for (int i = 0; i < 5; ++i)
					{
						for (auto& skill_icon : m_Tabs.getTabByIndex(i)->m_TabContent->m_Renderables)
							if (skill_icon->getSkillId() == skill_id)
							{
								return skill_icon.get();
							}
						//(*m_Tabs->getTabByIndex(0)->m_TabContent->m_Renderables[skill_id]).set_level(alloc);
					}
					return nullptr;
			}
			//SRL::SkillInfo * getSkillInfo(unsigned int skill_id)
			//{
			//	for (int i = 0; i < 5; ++i)
			//	{
			//		for (const auto& skill_icon : m_Tabs->getTabByIndex(0)->m_TabContent->m_Renderables)
			//			if (skill_icon->getSkillId() == skill_id)
			//			{
			//				return skill_icon->get_skill_info();
			//			}
			//	}
			//	return nullptr;
			//}
			//void add_skill(SRL::SkillData skill_data, unsigned tab)
			//{
			//	UiTab<skills::SkillIcon> * rel_tab = m_Tabs->getTabByIndex(tab);
			//	skills::SkillIcon * skill = new skills::SkillIcon{ skill_data, {11, 172 - float(36 * rel_tab->m_TabContent->m_Renderables.size())} };
			//	rel_tab->add_data(skill);
			//}
		};

	}
}
