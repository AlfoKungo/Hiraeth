#include "label.h"

namespace hiraeth
{
	namespace graphics
	{
		Label::Label(std::string text, float x, float y, maths::vec4 color)
			: Renderable2D(), m_Text(text)
		{

		}

		void Label::submit(Renderer2D* renderer) const
		{
			//renderer->drawString(m_Text, m_Bounds.x, m_Bounds.y, m_Color);
		}
	}
}
