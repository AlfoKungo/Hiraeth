#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/sprited_renderable.h"
#include "srl/quest_data.h"
#include "ui_basic/ui_tabs.h"

namespace hiraeth {
	namespace ui {

#define TAB_BEG 0
#define TAB_END 3

		class UiQuests : public UiWindow
		{
		private:
			UiTabs m_Tabs;
		public:
			UiQuests(maths::vec2 pos, input::Controls control_key);
			void mouse_clicked(maths::vec2 mousePos);
			void mouse_released(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override {}
			void fillGroup();
		};

	}
}
