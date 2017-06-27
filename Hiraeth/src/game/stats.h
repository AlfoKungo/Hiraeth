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
			unsigned int &MaxHp, &Hp;
			Stats(unsigned int &max_hp, unsigned int &hp) : MaxHp(max_hp), Hp(hp) {}
			virtual ~Stats() {}
			virtual Damage getDamage() const = 0;
			virtual void causeDamage(Damage damage) = 0;

		};

	}
}
