#pragma once

#include <FreeImage.h>
#include <string>
#include <memory>
#include <GL/glew.h>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
//#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>
#include <sstream>
#include <fstream>
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
			int m_DataSize;
			BYTE * m_Pixels;
		public:
			explicit Texture(const std::string& filename);
			Texture(const std::string& filename, int id);
			Texture(const std::string& name, const std::string& filename);
			Texture(const std::string& name, const std::string& filename, int id);
			~Texture();
			void bind() const;
			void unbind() const;
			
			// use the commented version when you fix the TextureManager module to recieve name and not a file_name
			//inline const std::string& getName() const { return m_Name; }
			inline const std::string& getName() const { return m_FileName; }
			inline unsigned int getID() const { return m_TID; }
			inline unsigned int getWidth() const { return m_Width; }
			inline unsigned int getHeight() const { return m_Height; }
			inline maths::vec2 getSize() const { return maths::vec2(float(m_Width), float(m_Height)); }
		private:
			GLuint load();
			void serialize(std::string filename);
			GLuint deserialize(int id);
			void read_png(char* file_name);

			std::string slash2underscore(std::string text) {
				for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
					if (*it == '/') {
						*it = '_';
					}
				}
				return text;
			}

		};
	}
}