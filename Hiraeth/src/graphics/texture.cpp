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
			//serialize(filename);
			//m_TID = deserialize();
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
			std::ofstream file("texture.data");

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

		//this is a POC, currently unusable
#define ADV_SERIALIZE
#ifdef ADV_SERIALIZE
		GLuint Texture::deserialize(int id)
		{
			GLuint result = 0;
			std::ifstream in("serialized/map_texture.data", std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				cereal::BinaryInputArchive ar(in);
				int offset;
				in.seekg(id * sizeof(int));
				ar(offset);
				in.seekg(offset);
				ar(m_Width, m_Height, m_DataSize);
				BYTE* newpixels = new BYTE[m_DataSize];
				ar(cereal::binary_data(newpixels, m_DataSize));

				glGenTextures(1, &result);
				glBindTexture(GL_TEXTURE_2D, result);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, newpixels);
				glBindTexture(GL_TEXTURE_2D, 0);

				delete[] newpixels;
			}

			return result;
		}
#endif // ADV_SERIALIZE

#ifndef ADV_SERIALIZE
		void Texture::serialize(std::string filename)
		{
			GLsizei width, height;
			BYTE* pixels = load_image(filename.c_str(), &width, &height);
			int dataSize = (sizeof(pixels) / sizeof(pixels[0])) * width * height;

//#define CHECK_ARRAY
#ifdef CHECK_ARRAY
			m_Pixels = new BYTE[dataSize];
			memcpy(m_Pixels, pixels, dataSize);
#endif // CHECK_ARRAY

			std::ofstream out("serialized/texture_" + m_Name + ".data", std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				//cereal::PortableBinaryOutputArchive ar(out, cereal::PortableBinaryOutputArchive::Options::BigEndian());
				cereal::BinaryOutputArchive ar(out);
				//out.open("serialized/texture_" + m_Name + ".data", std::ios::binary);
				//out.write(reinterpret_cast<char*>(pixels), m_DataSize);
				out.seekp(0);
				ar(width, height, dataSize, cereal::binary_data(pixels, dataSize));
			}

			delete[] pixels;
		}

		//this is a POC, currently unusable
		GLuint Texture::deserialize()
		{
			GLuint result;
			std::ifstream in("serialized/texture_" + m_Name + ".data", std::ios::in | std::ios::binary);
			//in.open
			if (in.is_open())
			{
				//cereal::PortableBinaryInputArchive ar(in, cereal::PortableBinaryInputArchive::Options::LittleEndian());
				cereal::BinaryInputArchive ar(in);
				in.seekg(0);
				ar(m_Width, m_Height, m_DataSize);
				BYTE* newpixels = new BYTE[m_DataSize];
				ar(cereal::binary_data(newpixels, m_DataSize));

#ifdef CHECK_ARRAY
				int n;
				for (int i = 0; i < m_DataSize; i++)
				{
					if (m_Pixels[i] != newpixels[i])
						n = 10;
				}
#endif // CHECK_ARRAY

			glGenTextures(1, &result);
			glBindTexture(GL_TEXTURE_2D, result);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, newpixels);
			glBindTexture(GL_TEXTURE_2D, 0);

			delete[] newpixels;
			return result;
			}
			return 0;
		}
#endif // N_ADV_SERIALIZE

		//this is a POC, currently unusable
		void Texture::read_png(char* file_name)
		{

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