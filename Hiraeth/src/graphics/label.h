#pragma once

#include <string>

#include "renderable2d.h"
#include "font_manager.h"

namespace hiraeth
{
	namespace graphics
	{
		class Label : public Renderable2D
		{
		//public:
		private:
			Font* m_Font;
			//float x, y;
			std::string m_Text;
		public:
			Label(Font* font, std::string text, float x, float y, unsigned int color);
			Label(const std::string& font, std::string text, float x, float y, unsigned int color);
			Label(const std::string& font, unsigned int size, std::string text, float x, float y, unsigned int color);
			Label(const std::string& font, unsigned int size, int number, float x, float y, unsigned int color);
			void draw(Renderer2D* renderer) const override;
			void setText(const std::string& new_text) { m_Text = new_text; }
			void setText(unsigned int new_text) { m_Text = std::to_string(new_text); }
		};
	}
}