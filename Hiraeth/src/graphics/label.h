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
			std::string m_Text;
			Alignment m_Alignment;
			maths::vec2 m_AlignmentOffset;
		public:
			Label(Font* font, std::string text, maths::vec2 pos, unsigned int color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& font, std::string text, maths::vec2 pos, unsigned int color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& font, unsigned int size, std::string text, maths::vec2 pos, unsigned int color, Alignment alignment = Alignment::LEFT);
			Label(const std::string& font, unsigned int size, int number, maths::vec2 pos, unsigned int color, Alignment alignment = Alignment::LEFT);
			void draw(Renderer* renderer) const override;
			void setText(const std::string& new_text);
			void setText(unsigned int new_text) { setText(std::to_string(new_text)); }
			void setSize(unsigned int new_size);
			std::string getText() const;
			void addLine(const std::string& new_line_text);
			maths::vec2 getSize() const override;
			maths::vec2 getSize(unsigned int length) const;
		private:
			void updateBounds();
		};
	}
}