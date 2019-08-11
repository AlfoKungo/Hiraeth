#pragma once

//#include <memory>
#include <cereal/archives/binary.hpp>
#include "ImageLoad.h"

namespace SRL {
	struct TextureData
	{
		int width, height;
		BYTE* pic = nullptr;
	private: 
		int texture_data_size{};
	public:
		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(width, height, texture_data_size);
			if (pic == nullptr)
				pic = new BYTE[texture_data_size];
			ar(cereal::binary_data(pic, texture_data_size));
		}
		void load_texture(std::string img_path)
		{
			pic = load_image(img_path.c_str(), &width, &height);
			//texture_data_size = (sizeof(pic) / sizeof(pic[0])) * width * height ; // 32-bit
			//texture_data_size = (sizeof(pic) / sizeof(pic[0])) * width * height / 2; // 64-bit
			texture_data_size = 4 * width * height / 2; 
		}
	};


}