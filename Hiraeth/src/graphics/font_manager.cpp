#include "hrth_pch.h"
#include "font_manager.h"

namespace hiraeth {
	namespace graphics {
		
		std::vector<Font*> FontManager::m_Fonts;
		Font* FontManager::add(Font* font)
		{
			m_Fonts.push_back(font);
			return font;
		}

		Font* FontManager::get(const std::string& name)
		{
			for (Font* font : m_Fonts)
			{
				if (font->getName() == name)
					return font;
			}
			return nullptr;
		}

		Font* FontManager::get(const std::string& name, unsigned int size)
		{
			for (Font* font : m_Fonts)
			{
				if (font->getSize() == size && font->getName() == name)
					return font;
			}
			return add(new graphics::Font(name, name + ".ttf", size));
			//return nullptr;
		}
		
		void FontManager::clean()
		{
			for (int i = 0; i < m_Fonts.size(); i++)
				delete m_Fonts[i];
		}
	}
}