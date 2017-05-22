#include "texturemanager.h"
#include <iostream>

namespace hiraeth {
	namespace graphics {

		std::vector<Texture*> TextureManager::m_Textures;

		Texture* TextureManager::Load(const std::string& name)
		{

			for (Texture* texture : m_Textures)
			{
				if (texture->getName() == name)
					return texture;
			}
			m_Textures.push_back(new Texture(name));
			std::cout << "Tid is " +std::to_string( m_Textures.back()->getID()) + " and name is: " + name << std::endl;
				return m_Textures.back();
		}

		Texture* TextureManager::Get(const std::string& name)
		{
			for (Texture* texture : m_Textures)
			{
				if (texture->getName() == name)
					return texture;
			}
			return nullptr;
		}

		void TextureManager::Clean()
		{
			for (unsigned int i = 0; i < m_Textures.size(); i++)
				delete m_Textures[i];
		}

	}
}