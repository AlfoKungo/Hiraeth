#pragma once

#include <vector>

namespace hiraeth {
	namespace audio {

		class AudioManager
		{
		private:
			static std::vector<Font*> m_Fonts;
		public:
			static void add(Font* font);
			static Font* get(const std::string& name);
			static Font* get(const std::string& name, unsigned int size);
			static void clean();
		private:
			FontManager();
		};
	}
}
