#pragma once
#include <string>
#include "texture_data.h"
#include "maths/maths.h"
#include <variant>

namespace SRL {

	struct NpcInfo
	{
		std::string npc_name;
		unsigned int npc_foothold;
		float npc_x_value;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_name), CEREAL_NVP(npc_foothold), CEREAL_NVP(npc_x_value));
		}
	};


	struct NpcData
	{
		NpcInfo info;
		TextureData texture_data;
		unsigned int npc_frames_amount = 1;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(info), CEREAL_NVP(texture_data), CEREAL_NVP(npc_frames_amount));
		}
	};

	struct DialogTreeE
	{
		std::vector<std::variant<std::string, unsigned int>> m_Strings;

	};

}
