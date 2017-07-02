#pragma once

#include "utils\static_timer.h"

namespace hiraeth {

	class ATimer
	{
	private:
		float m_Timer;
	public:
		ATimer()
			: m_Timer(0)
		{
		}
		
		bool isExpired()
		{
			return (StaticTimer::timer.elapsed() - m_Timer > 0.0f);
		}

		void reSet(float time_to_wait)
		{
			m_Timer = StaticTimer::timer.elapsed() + time_to_wait;
		}

		float timeRemain()
		{
			return (m_Timer - StaticTimer::timer.elapsed());
		}

	};
}
