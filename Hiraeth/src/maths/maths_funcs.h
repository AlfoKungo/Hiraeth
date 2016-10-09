#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace hiraeth {
	namespace maths {
		inline float toRadiants(float degrees)
		{
			return degrees * (M_PI / 180.0f);
		}
	}
}
