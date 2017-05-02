#pragma once

#include <string>

namespace hiraeth {
	namespace audio {

		class Sound
		{
		private:
			std::string m_Name;
			std::string m_FileName;
		public:
			Sound(const std::string& name, const std::string& filename);
			void play();
			void loop();
			void pause();
			void stop();

			inline const std::string& getName() const { return m_Name; }
			//inline const std::string& getFilename() const { return m_Filename; }

		};

	}
}