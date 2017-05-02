#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
#include "../../ext/freetype-gl/freetype-gl.h"
#include <ga.h>

namespace hiraeth {
	namespace graphics {

		class Font
		{
		private:
			ftgl::texture_atlas_t* m_FTAtlas;
			ftgl::texture_font_t* m_FTFont;
			unsigned int m_Size;

			std::string m_Name;
			std::string m_Filename;
		public:
			Font(const std::string& name, const std::string& filename, int size);
			~Font();


			inline ftgl::texture_font_t* getFTFont() const { return m_FTFont; }
			
			inline const unsigned int getID() const { return m_FTAtlas->id; }
			inline const std::string& getName() const { return m_Name; }
			inline const std::string& getFilename() const { return m_Filename; }
			inline const int getSize() const { return m_Size; }

			void set_rendermode(ftgl::rendermode_t rendermode);
			void set_rendermode(ftgl::rendermode_t rendermode, float outline_thickness);
			void submitFont() const;
		};

	}
}