#pragma once
#include <fstream>
#ifdef SERIALIZE_MACRO
#include <cereal/archives/json.hpp>
#endif
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
#ifdef SERIALIZE_MACRO
			cereal::JSONInputArchive arin(in);
			arin(animation_data); // This is because of a weird bug caused by the includes to json
#endif
			animation_texture.load_texture(path + "\\" +animation_type + ".png");
		}
	};
}
