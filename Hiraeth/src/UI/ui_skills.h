#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/sprited_renderable.h"
#include "UI/ui_basic/ui_tabs.h"

namespace hiraeth {
	namespace ui {

#define TAB_BEG 0
#define TAB_END 4

		class UiSkills : public UiWindow
		{
		private:
			std::map<unsigned int, std::unique_ptr<graphics::Group>> m_SkillTabs;
			unsigned int m_Tab;
			UiTabs m_Tabs;

		public:
			UiSkills(maths::vec2 pos, input::Controls control_key);
			void mouse_clicked(maths::vec2 mousePos);
			void mouse_released(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my) override {}
			void fillGroup();
		};

	}
}
