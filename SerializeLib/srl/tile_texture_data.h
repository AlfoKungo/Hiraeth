#pragma once

#include <memory>
#include <cereal/archives/binary.hpp>
#include "texture_data.h"
#include "maths/maths.h"

namespace SRL {
	struct TileUv
	{
		maths::vec2 UvPos;
		maths::vec2 UvSize;
		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(UvPos, UvSize);
		}
	};

	struct TileTextureData
	{
		std::vector<TileUv> TilesUV;
		TextureData texture_data;
		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(TilesUV, texture_data);
		}
	};

}