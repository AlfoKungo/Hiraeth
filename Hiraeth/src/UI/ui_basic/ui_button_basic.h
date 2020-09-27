#pragma once
#include "ui_button.h"
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"

namespace hiraeth::ui {

	class UiButtonBasic : public UiButton
	{
	private:
		graphics::Sprite m_Enabled, m_MouseOver, m_Disabled, m_Pressed;
	public:
		UiButtonBasic(maths::vec2 pos, const std::string& window_name, const std::string& button_name, std::function<void()>&& func_to_call)
			: UiButton(&m_Enabled, func_to_call),
			m_Enabled(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".normal.0.png")),
			//m_Enabled(maths::vec2(120, 120), 4, 0.15f, true, 
			//	graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".normal.0.png"), maths::vec2(0)),
			m_MouseOver(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".mouseOver.0.png")),
			m_Disabled(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".disabled.0.png")),
			m_Pressed(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".pressed.0.png"))
		{

		}

		void onMove(maths::vec2 mouse_pos) override
		{
			if (isContains(mouse_pos))
				m_Rend = &m_MouseOver;
			else
				m_Rend = &m_Enabled;
		}
	};
}

