#pragma once
#include "ui_window.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"

namespace hiraeth {
	namespace ui {

		class UiKeyConfig : public UiWindow
		{
		public:
			UiKeyConfig(maths::vec2 pos, UiKey control_key)
				: UiWindow(maths::Rectangle{pos, {622,374}}, control_key)
			{
				
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/KeyConfig/KeyConfig.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 117), graphics::TextureManager::Load("Assets/UI/KeyConfig/KeyConfig.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(10, 145), graphics::TextureManager::Load("Assets/UI/KeyConfig/KeyConfig.backgrnd3.png")));
			}
			
			void mouse_left_clicked(maths::vec2 mousePos) override {}
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override {}
			//void fillGroup() {}
		};
	}
}
