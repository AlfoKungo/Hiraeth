#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"
#include "graphics/label.h"
#include "item/item.h"
#include "basic/EventManager.h"

namespace hiraeth {
	namespace ui {
		class UiInventory : public UiWindow
		{
		private:
			enum Tab
			{
				Equip = 0,
				Use = 1,
				SetUp = 2,
				Etc = 3,
				Cash = 4,
			} m_Tab;
			const std::vector<item::Item*>& m_Items;
		public:
			UiInventory(maths::vec2 pos, input::Controls control_key, const std::vector<item::Item*>& items);
			void fillGroup();
			void mouse_clicked(maths::vec2 mousePos) override;
			void mouse_released(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my) override {}
			//void addItem(item::Item * item) { m_Items.push_back(item); }
		};

	}
}