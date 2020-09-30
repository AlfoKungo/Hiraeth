#include "hrth_pch.h"
#include "label.h"

namespace hiraeth
{
	namespace graphics
	{
		Label::Label(Font* font, std::string text, maths::vec2 pos, unsigned int color, Alignment alignment)
			: Renderable(), m_Font(font), m_Text(text), m_Alignment(alignment)
		{
			updateBounds();
			m_Bounds.position = pos;
			m_Color = color;
		}

		Label::Label(const std::string& font, std::string text, maths::vec2 pos, unsigned int color, Alignment alignment)
			: Renderable(), m_Font(FontManager::get(font)), m_Text(text), m_Alignment(alignment)
		{
			updateBounds();
			m_Bounds.position = pos;
			m_Color = color;
		}

		Label::Label(const std::string& font, unsigned int size, std::string text, maths::vec2 pos, unsigned int color, Alignment alignment)
			: Renderable(), m_Font(FontManager::get(font, size)), m_Text(text), m_Alignment(alignment)
		{
			updateBounds();
			m_Bounds.position = pos;
			m_Color = color;
		}
		Label::Label(const std::string& font, unsigned int size, int number, maths::vec2 pos, unsigned int color, Alignment alignment)
			: Label(font, size, std::to_string(number), pos, color, alignment)
		{
		}

		void Label::draw(Renderer* renderer) const
		{
			renderer->drawString(*m_Font, m_Text, m_Bounds.position + m_AlignmentOffset, m_Color);
		}  

		void Label::setText(const std::string& new_text)
		{
			m_Text = new_text;
			updateBounds();
		}

		void Label::setSize(unsigned int new_size)
		{
			m_Font = FontManager::get(m_Font->getName(), new_size);
			updateBounds();
		}

		std::string Label::getText() const
		{
			return m_Text;
		}

		void Label::addLine(const std::string& new_line_text)
		{
			m_Text = m_Text + '\n' + new_line_text;
		}

		maths::vec2 Label::getSize() const
		{
			return m_Font->GetSize(m_Text);
		}

		maths::vec2 Label::getSize(unsigned length) const
		{
			auto sstr = m_Text.substr(0, length);
			return m_Font->GetSize(sstr);
		}

		void Label::updateBounds()
		{
			using namespace maths;

			vec2 size = m_Font->GetSize(m_Text);
			m_Bounds.size = size;
			switch (m_Alignment)
			{
			case Alignment::LEFT:
				m_AlignmentOffset.x = 0;
				break;
			case Alignment::CENTER:
				m_AlignmentOffset.x = -size.x / 2;
				break;
			case Alignment::RIGHT:
				m_AlignmentOffset.x = -size.x;
				break;
			}
		}

	}
}
