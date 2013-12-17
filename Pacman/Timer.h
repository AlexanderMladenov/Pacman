#ifndef __TIMER_H
#define __TIMER_H


namespace Pacman
{
	// High-precision timer class that uses QueryPerformanceFrequency to calculate time.
	class Timer 
	{
		static double PCFreq;
		static __int64 CounterStart;

	public:
		static void Initialize();
		static double GetTime();

	};
}
#endif