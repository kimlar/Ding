#pragma once

#include <windows.h>

class Timer
{
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	double elapsedTime;

public:
	void StartTimer()
	{

		// get ticks per second
		QueryPerformanceFrequency(&frequency);

		// start timer
		QueryPerformanceCounter(&t1);
	}

	double StopTimer()
	{
		// stop timer
		QueryPerformanceCounter(&t2);

		// compute and print the elapsed time in millisec
		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		//cout << elapsedTime << " ms.\n";
		
		return elapsedTime;
	}
};
