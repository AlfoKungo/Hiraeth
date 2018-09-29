#pragma once
#include <cereal/cereal.hpp>

namespace SRL
{
	struct TimedValue
	{
		int value, duration;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(value), CEREAL_NVP(duration));
		}
	};

}
