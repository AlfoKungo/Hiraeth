#pragma once

#include "static_timer.h"

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
		explicit ATimer(float time_to_wait)
			: m_Timer(StaticTimer::timer.elapsed() + time_to_wait)
		{
		}
		
		bool hasExpired() const
		{
			return (StaticTimer::timer.elapsed() - m_Timer > 0.0f);
		}

		void reSet(float time_to_wait)
		{
			m_Timer = StaticTimer::timer.elapsed() + time_to_wait;
		}

		float timeRemain() const
		{
			return (m_Timer - StaticTimer::timer.elapsed());
		}

	};
	class ACounter
	{
	private:
		float m_Counter;
	public:
		ACounter()
			: m_Counter(StaticTimer::timer.elapsed())
		{
		}
		
		bool hasExpired(float time_passed) const
		{
			return (StaticTimer::timer.elapsed() - m_Counter > time_passed);
		}

		void reSet()
		{
			m_Counter = StaticTimer::timer.elapsed();
		}

		float timePassed() const
		{
			return StaticTimer::timer.elapsed() - m_Counter;
		}

	};
}
