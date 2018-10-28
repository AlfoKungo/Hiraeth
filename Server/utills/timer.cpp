#include "timer.h"
#include <algorithm>
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <Windows.h>

namespace hiraeth {

	struct Members
	{
		LARGE_INTEGER m_Start;
		double m_Frequency;
	};

	Timer::Timer()
		: m_Members(new (m_Reserved) Members())
		//: m_Members(new Members(m_Reserved))
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_Members->m_Frequency = 1.0 / frequency.QuadPart;

		Reset();
	}

	void Timer::Reset()
	{
		QueryPerformanceCounter(&m_Members->m_Start);
	}

	float Timer::elapsed()
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		unsigned long long cycles = current.QuadPart - m_Members->m_Start.QuadPart;
		return (float)(cycles * m_Members->m_Frequency);
	}

	float Timer::ElapsedMillis()
	{
		return elapsed() * 1000.0f;
	}

}