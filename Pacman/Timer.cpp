#include "Timer.h"
#include <stdexcept>
#include <windows.h>
namespace Pacman
{
	double Timer::PCFreq = 0;
	__int64 Timer::CounterStart = 0;

	void Timer::Initialize()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			throw std::runtime_error("QueryPerformanceFrequency failed!");

		PCFreq = double(li.QuadPart); // returns seconds at the moment. to acquire miliseconds, divide by 1000.0

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}

	double Timer::GetTime()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - CounterStart) / PCFreq;
	}
}