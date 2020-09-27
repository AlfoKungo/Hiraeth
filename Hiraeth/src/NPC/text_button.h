#pragma once
#include "UI/ui_basic/ui_button.h"
#include "graphics/label.h"

namespace hiraeth::npc {

	class TextButton : public ui::UiButton
	{
		graphics::Label m_Text;
	public:
		TextButton(maths::vec2 pos, const std::string& text)
			: UiButton(&m_Text, nullptr),
			m_Text("arial",19, text, pos, 0xffffffff)
		{

		}

		void onMove(maths::vec2 mouse_pos) override
		{
			if (isContains(mouse_pos))
			{
				m_Text.setSize(20);
				m_Text.setColor(0xffffea51);
			}
			else
			{
				m_Text.setSize(19);
				m_Text.setColor(0xffffffff);
			}
		}
	};

}
