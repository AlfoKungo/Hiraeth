#pragma once

#include <FreeImage.h>
#include <string>
#include <GL/glew.h>
#include "../utils/ImageLoad.h"

namespace hiraeth {
	namespace graphics {

		class Texture
		{
		private:
			std::string m_Name, m_FileName;
			GLuint m_TID;
			GLsizei m_Width, m_Height;
		public:
			Texture(const std::string& filename);
			Texture(const std::string& name, const std::string& filename);
			~Texture();
			void bind() const;
			void unbind() const;

			inline const std::string& getName() { return m_Name; }
			inline const unsigned int getID() { return m_TID; }
			inline const unsigned int getWidth() { return m_Width; }
			inline const unsigned int getHeight() { return m_Height; }
		private:
			GLuint load();

		};
	}
}