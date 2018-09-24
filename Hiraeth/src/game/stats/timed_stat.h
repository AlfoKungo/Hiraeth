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
			//ItemIdType item_id;
			ATimer m_Timer;

		public:
			TimedStat(float* value, SRL::TimedValue timed_value)
				: m_Value{ value },
				m_ValueInc(timed_value.value),
				m_Timer{ timed_value.duration }
			{
				(*value) += float(timed_value.value);
			}

			bool checkAndCancel()
			{
				if (m_Timer.isExpired())
				{
					(*m_Value) -= m_ValueInc;
					return true;
				}
				return false;
			}
		};
	}
}
