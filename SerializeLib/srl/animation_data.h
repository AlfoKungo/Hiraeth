#pragma once
#include <fstream>
#include <cereal/archives/json.hpp>
#include "texture_data.h"
#include "sprite_data.h"

namespace SRL
{
	struct FullAnimationData
	{
		AnimationData animation_data;
		TextureData animation_texture;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(animation_data), CEREAL_NVP(animation_texture));
		}
		void load_data(const std::string& path, const std::string& animation_type)
		{
			std::ifstream in(path + "\\" + animation_type +".json", std::ios::in);
			cereal::JSONInputArchive arin(in);
#ifdef SERIALIZE_MACRO
			arin(animation_data); // This is because of a weird bug caused by the includes to json
#endif
			animation_texture.load_texture(path + "\\" +animation_type + ".png");
		}
	};
}
