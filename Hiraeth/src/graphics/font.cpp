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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, m_FTAtlas->width, m_FTAtlas->height,
				0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, m_FTAtlas->data);
		}
	}
}
