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
			virtual Damage getDamage() const = 0;
			virtual void causeDamage(Damage damage) = 0;

		};


		class NpcStats : public Stats
		{
			struct NpcStatsStruct
			{
				unsigned int MaxHp; unsigned int Hp; float Speed; float Jump;
			};
			NpcStatsStruct m_Stats;
			NpcStats() :
				Stats{ m_Stats.MaxHp, m_Stats.Hp, m_Stats.Speed, m_Stats.Jump },
				m_Stats{ 0, 0, 0, 0 }
			{

			}
			Damage getDamage() const override
			{
				return { 0,0 };
			}
			void causeDamage(Damage damage) override {}
		};

	}
}
