#include "texturemanager.h"

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