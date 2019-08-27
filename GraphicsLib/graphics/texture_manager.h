#pragma once

#include "texture.h"
#include <vector>
#include <string>

namespace hiraeth {
	namespace graphics {

		class TextureManager
		{
		private:
			static std::vector<Texture*> m_Textures;
		public:
			static Texture* Load(const std::string& name);
			static Texture* Load(const std::string& name, SRL::TextureData texture_data);
			static Texture* Get(const std::string& name);
			static void Clean();
		private:
			TextureManager() { }
		};

	}
}
