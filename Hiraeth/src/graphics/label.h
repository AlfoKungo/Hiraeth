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
			std::string m_Text;
			float x, y;
		public:
			Label(Font* font, std::string text, float x, float y, unsigned int color);
			Label(const std::string& font, std::string text, float x, float y, unsigned int color);
			Label(const std::string& font, unsigned int size, std::string text, float x, float y, unsigned int color);
			Label(const std::string& font, unsigned int size, int number, float x, float y, unsigned int color);
			void submit(Renderer2D* renderer) const override;
		};
	}
}