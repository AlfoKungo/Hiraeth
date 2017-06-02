#pragma once

#include <FreeImage.h>
#include <string>
#include <GL/glew.h>
#include "maths/maths.h"
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
			explicit Texture(const std::string& filename);
			Texture(const std::string& name, const std::string& filename);
			~Texture();
			void bind() const;
			void unbind() const;

			inline const std::string& getName() const { return m_Name; }
			inline unsigned int getID() const { return m_TID; }
			inline unsigned int getWidth() const { return m_Width; }
			inline unsigned int getHeight() const { return m_Height; }
			inline maths::vec2 getSize() const { return maths::vec2(m_Width, m_Height); }
		private:
			GLuint load();

		};
	}
}