#pragma once
#include "ui_button.h"
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"

namespace hiraeth {
	namespace ui {

		class UiButtonNormal : public UiButton
		{
		private:
			graphics::Sprite m_Enabled, m_MouseOver, m_Disabled, m_Pressed;
		public:
			UiButtonNormal(maths::vec2 pos, std::string window_name, std::string button_name, std::function<void()>&& func_to_call)
				: UiButton(&m_Enabled,func_to_call),
				m_Enabled(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".normal.0.png")),
				//m_Enabled(maths::vec2(120, 120), 4, 0.15f, true, 
				//	graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".normal.0.png"), maths::vec2(0)),
				m_MouseOver(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".mouseOver.0.png")),
				m_Disabled(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".disabled.0.png")),
				m_Pressed(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".pressed.0.png"))
			{

			}
		};
	}
}
