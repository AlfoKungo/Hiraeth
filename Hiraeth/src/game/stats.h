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
			virtual ~Stats() = default;
			virtual float getSpeed() const = 0;
			virtual float getJump() const = 0;
			virtual Damage getDamage() const = 0;
			virtual void causeDamage(Damage damage) = 0;
			virtual bool consumeMana(unsigned int mp) = 0;
			virtual void recoverHp(unsigned int heal_amount) = 0;

		};

	}
}
