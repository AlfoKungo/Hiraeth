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
			float &Speed, &Jump;
			Stats(unsigned int &max_hp, unsigned int &hp, float &speed, float &jump) 
				: MaxHp(max_hp), Hp(hp), Speed(speed), Jump(jump) {}
			virtual ~Stats() {}
			virtual Damage getDamage() const = 0;
			virtual void causeDamage(Damage damage) = 0;

		};

	}
}
