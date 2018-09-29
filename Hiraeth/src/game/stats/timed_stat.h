#pragma once
#include "basic/a_timer.h"
#include "srl/item_data.h"

namespace hiraeth
{
	namespace stats {

		class TimedStat
		{
			float * m_Value;
			float m_ValueInc;
			ATimer m_Timer;

		public:
			TimedStat(float* value, SRL::TimedValue timed_value)
				: m_Value{ value },
				m_ValueInc(timed_value.value),
				m_Timer{ timed_value.duration }
			{
				(*value) += float(timed_value.value);
			}

			TimedStat(const TimedStat& other) = default;

			TimedStat & operator = (TimedStat other)
			{
				std::swap(m_Value, other.m_Value);
				std::swap(m_ValueInc, other.m_ValueInc);
				std::swap(m_Timer, other.m_Timer);
				return *this;
			}

			~TimedStat()
			{
				(*m_Value) -= m_ValueInc;
			}

			bool checkTimer() const
			{
				return (m_Timer.hasExpired());
			}
		};


	}
}
