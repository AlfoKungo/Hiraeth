#pragma once
#include <cereal/cereal.hpp>
#include "maths/vec2.h"
#include "maths/rectangle.h"
#include <map>
#include <cereal/archives/json.hpp>
#include <vector>

namespace SRL
{
	enum MoveState {
		Stand = 0,
		Walk = 1,
		Hit = 2,
		Die = 3,
	};

	struct SpritedFrameData
	{
		maths::Rectangle frame_box;
		maths::vec2 origin;
		float delay{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(frame_box), CEREAL_NVP(origin), CEREAL_NVP(delay));
		}
	};

	struct AnimationData
	{
		std::vector<SpritedFrameData> frames_data;
		bool is_loop{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(frames_data), CEREAL_NVP(is_loop));
		}
	};

}
