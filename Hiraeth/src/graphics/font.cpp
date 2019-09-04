#include "font.h"

namespace hiraeth {
	namespace graphics {

		Font::Font(const std::string& name, const std::string& filename, int size)
			:m_Name(name), m_Filename(filename), m_Size(size)
		{

			m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
			m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas, size, filename.c_str());
		}

		Font::~Font()
		{
			ftgl::texture_font_delete(m_FTFont);
			ftgl::texture_atlas_delete(m_FTAtlas);
		}

		void Font::set_rendermode(ftgl::rendermode_t rendermode)
		{
			m_FTFont->rendermode = rendermode;
		}

		void Font::set_rendermode(ftgl::rendermode_t rendermode, float outline_thickness)
		{
			m_FTFont->rendermode = rendermode;
			m_FTFont->outline_thickness = outline_thickness;
		}

		void Font::submitFont() const
		{

			if (!getID())
			{
				glGenTextures(1, &m_FTAtlas->id);
			}
			glBindTexture(GL_TEXTURE_2D, getID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, m_FTAtlas->width, m_FTAtlas->height,
				0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, m_FTAtlas->data);
		}

		float Font::GetWidth(const std::string& text) const
		{
			using namespace ftgl;

			float width = 0.0f;
			const maths::vec2& scale = maths::vec2(1);
			for (int i = 0; i < text.size(); i++)
			{
				texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, &text[i]);
				if (i > 0)
				{
					float kerning = texture_glyph_get_kerning(glyph, &text[i - 1]);
					width += kerning / scale.x;
				}
				width += glyph->advance_x / scale.x;
			}
			return width;
		}

		float Font::GetHeight(const std::string& text) const
		{
			using namespace ftgl;

			const maths::vec2& scale = maths::vec2(1);
			float min = 0.0f;
			float max = 0.0f;
			for (int i = 0; i < text.size(); i++)
			{
				texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, &text[i]);
				float height = glyph->height / scale.y;
				float offset = glyph->offset_y / scale.y - height;
				if (offset < min)
					min = offset;
				if (height > max)
					max = height;
			}
			return abs(min) + abs(max);

		}

		maths::vec2 Font::GetSize(const std::string& text) const
		{
			return maths::vec2(GetWidth(text), GetHeight(text));
		}
	}
}
