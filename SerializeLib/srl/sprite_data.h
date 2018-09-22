#pragma once
#include <cereal/cereal.hpp>
#include "maths/vec2.h"
#include "maths/rectangle.h"
#include <map>

namespace SRL
{
	enum MoveState {
		Stand = 1,
		Walk = 2,
		Hit = 3
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

	struct CreatureSprites
	{
		std::map<MoveState, AnimationData> sprited_data;
		maths::vec2 hit_box;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(sprited_data), CEREAL_NVP(hit_box));
		}
	};
}
