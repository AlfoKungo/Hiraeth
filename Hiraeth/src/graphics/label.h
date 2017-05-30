#pragma once

#include <string>

#include "renderable.h"
#include "font_manager.h"

namespace hiraeth
{
	namespace graphics
	{
		class Label : public Renderable
		{
		public:
			enum class Alignment
			{
				LEFT = 0, CENTER, RIGHT
			};
		private:
			Font* m_Font;
			//float x, y;
			std::string m_Text;
			Alignment m_Alignment;
			maths::vec2 m_AlignmentOffset;
		public:
			Label(Font* font, std::string text, float x, float y, unsigned int color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& font, std::string text, float x, float y, unsigned int color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& font, unsigned int size, std::string text, float x, float y, unsigned int color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& font, unsigned int size, int number, float x, float y, unsigned int color, Alignment alignment = Alignment::LEFT);
			void draw(Renderer* renderer) const override;
			void setText(const std::string& new_text);
			void setText(unsigned int new_text) { setText(std::to_string(new_text)); }
		private:
			void updateBounds();
		};
	}
}