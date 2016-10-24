#pragma once

//#define WIN64_LEAN_AND_MEAN
#include <Windows.h>


//#include <WinDef.h>
//#include <WinGDI.h>
//#include <GL/gl.h>

namespace hiraeth {

	struct Members;
	class Timer
	{
	private:
		unsigned char m_Reserved[32];
		Members* m_Members;
	public:
		// Creates and starts timer
		Timer();
		// Resets and restarts timer
		virtual void Reset();
		// Returns time in seconds
		virtual float elapsed();
		// Returns time in milliseconds
		virtual float ElapsedMillis();
	//private:
	//	LARGE_INTEGER m_Start;
	//	double m_Frequency;
	//public:
	//	Timer()
	//	{
	//		LARGE_INTEGER frequency;
	//		QueryPerformanceFrequency(&frequency);
	//		m_Frequency = 1.0 / frequency.QuadPart;
	//		QueryPerformanceCounter(&m_Start);
	//	}

	//	void reset()
	//	{
	//		QueryPerformanceCounter(&m_Start);
	//	}

	//	float elapsed()
	//	{
	//		LARGE_INTEGER current;
	//		QueryPerformanceCounter(&current);
	//		unsigned __int64 cycles = current.QuadPart - m_Start.QuadPart;
	//		return (float)(cycles * m_Frequency);
		//}
	};
}