#pragma once
#include <string>
#include "texture_data.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include "maths/maths.h"
#include <variant>

namespace SRL {

#define DIALOG_TYPE const char
	DIALOG_TYPE
		RESPONSE_TYPE_NEXT = 0,
		RESPONSE_TYPE_ACCEPT = 1,
		RESPONSE_TYPE_CANCEL = 2,
		RESPONSE_TYPE_OK = 3;

	enum DialogType
	{
		quest
	};
	
	struct DialogStruct
	{
		std::string text;
		std::vector<char> buttons;
		//std::map<DialogType, std::variant<int>> props;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(text), CEREAL_NVP(buttons));
		}
	};
	
	struct NpcInfo
	{
		std::string npc_name;
		unsigned int npc_foothold;
		float npc_x_value;
		std::vector<std::string> say_lines;
		//std::vector<std::string> dialog_tree;
		std::map<unsigned int, std::vector<DialogStruct>> dialog_tree;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(npc_name), CEREAL_NVP(npc_foothold), CEREAL_NVP(npc_x_value),
				CEREAL_NVP(say_lines), CEREAL_NVP(dialog_tree));
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
