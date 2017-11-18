#include "texture.h"

namespace hiraeth {
	namespace graphics
	{
		Texture::Texture(const std::string& filename)
			: Texture(slash2underscore(filename), filename)
		{}

		Texture::Texture(const std::string& filename, int id)
			: Texture(slash2underscore(filename), filename, id)
		{}

		Texture::Texture(const std::string& name, const std::string& filename)
			: m_Name(name), m_FileName(filename)
		{
			m_TID = load();
		}

		Texture::Texture(const std::string& name, const std::string& filename, int id)
			: m_Name(name), m_FileName(filename)
		{
			m_TID = deserialize(id);
		}

		Texture::~Texture()
		{

		}

		GLuint Texture::load()
		{
			BYTE* pixels = load_image(m_FileName.c_str(), &m_Width, &m_Height);

			GLuint result;
			glGenTextures(1, &result);
			glBindTexture(GL_TEXTURE_2D, result);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
			glBindTexture(GL_TEXTURE_2D, 0);

			delete[] pixels;
			return result;
		}

		GLuint Texture::deserialize(int id)
		{
			GLuint result = 0;
			std::ifstream in("serialized/map.data", std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				cereal::BinaryInputArchive ar(in);
				int offset;
				in.seekg(id * sizeof(int) + 100 * sizeof(int));
				ar(offset);
				in.seekg(offset);
				TextureData m_Data;
				ar(m_Data);

				m_Width = m_Data.width;
				m_Height = m_Data.height;

				glGenTextures(1, &result);
				glBindTexture(GL_TEXTURE_2D, result);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_Data.pic);
				glBindTexture(GL_TEXTURE_2D, 0);

				delete[] m_Data.pic;
			}

			return result;
		}

		void Texture::bind() const
		{
			glBindTexture(GL_TEXTURE_2D, m_TID);
		}

		void Texture::unbind() const 
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}