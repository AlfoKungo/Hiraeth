#pragma once
#include "box_textured_full.h"
#include "label.h"

namespace hiraeth {
	namespace graphics {

		template <class BoxType = BoxTexturedFull>
		class TextBox : Renderable
		{
			BoxType m_Box;
			Label m_Label;
			maths::vec2 m_Pos;
		public:
			TextBox(maths::vec2 pos, std::string text, unsigned int text_color, std::string text_box_path)
				: m_Box{ pos + maths::vec2{0, 15}, 0, text_box_path },
			m_Label{"arial", 16, text, pos , text_color, Label::Alignment::CENTER},
			m_Pos(pos)
			{
				
			}
			void draw(Renderer* renderer) const override
			{
				m_Box.draw(renderer);
				m_Label.draw(renderer);
			}

			void setText(std::string new_text)
			{
				m_Label.setText(new_text);
				const auto text_size = m_Label.getSize();
				m_Label.setPosition(m_Pos +maths::vec2{ 0, text_size.y + 2 });
				m_Box.setSize(text_size + maths::vec2{ 20, 0 });
			}
		};
	}
}
