#include "label.h"

namespace hiraeth
{
	namespace graphics
	{
		Label::Label(Font* font, std::string text, float x, float y, unsigned int color)
			: Renderable2D(), m_Text(text), m_Font(font)
		{
			m_Bounds.x = x;
			m_Bounds.y = y;
			m_Color = color;
		}

		Label::Label(const std::string& font, std::string text, float x, float y, unsigned int color)
			: Renderable2D(), m_Text(text), m_Font(FontManager::get(font))
		{
			m_Bounds.x = x;
			m_Bounds.y = y;
			m_Color = color;
		}

		Label::Label(const std::string& font, unsigned int size, std::string text, float x, float y, unsigned int color)
			: Renderable2D(), m_Text(text), m_Font(FontManager::get(font, size))
		{
			m_Bounds.x = x;
			m_Bounds.y = y;
			m_Color = color;
		}
		Label::Label(const std::string& font, unsigned int size, int number, float x, float y, unsigned int color)
			: Label(font, size, std::to_string(number), x, y, color)
		{
		}

		void Label::submit(Renderer2D* renderer) const
		{
			renderer->drawString(*m_Font, m_Text, m_Bounds.position, m_Color);
		}
	}
}
