#pragma once
#include <string>
#include "texture_data.h"
#include "maths/maths.h"

namespace SRL {

	struct NpcInfo
	{
		std::string npc_name;
		unsigned int npc_foothold;
		float npc_x_value;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_name));
		}
	};


	struct NpcData
	{
		NpcInfo npc_info;
		TextureData texture_data;
		unsigned int npc_frames_amount = 1;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_info), CEREAL_NVP(texture_data), CEREAL_NVP(npc_frames_amount));
		}
	};

}