#pragma once
#include <string>

namespace hiraeth {
	namespace game {

		struct Damage
		{
			unsigned int RawDamage;
			unsigned int accuracy;
		};
		class Stats
		{
		public:
			Stats() {}
			virtual ~Stats() {}
			virtual Damage getDamage() const = 0;
			virtual void causeDamage(Damage damage) = 0;

		};

	}
}
