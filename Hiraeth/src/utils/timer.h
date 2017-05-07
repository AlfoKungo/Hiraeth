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
	};
}