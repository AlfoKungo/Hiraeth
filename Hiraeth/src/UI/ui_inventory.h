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
			const std::vector<item::Item*>& m_Items;
			item::Tab m_Tab;
			//std::map<item::Tab, graphics::Group> m_LayerItems;
			std::map<item::Tab, std::unique_ptr<graphics::Group>> m_LayerItems;
		public:
			UiInventory(maths::vec2 pos, input::Controls control_key, const std::vector<item::Item*>& items);
			void fillGroup();
			void mouse_clicked(maths::vec2 mousePos) override;
			void mouse_released(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my) override {}
			void fillLayer();
			//void addItem(item::Item * item) { m_Items.push_back(item); }
		};

	}
}