#include "Clock.h"

namespace Luna
{
	Clock::Clock()
	{
		SetThreadAffinityMask(GetCurrentThread(), 1); // Set the affinity mask to the first core
		QueryPerformanceFrequency(&m_freq); // Get the frequency of the performance counter

		Start(); // Start the clock
		Stop(); // Stop the clock
	}

	Clock::~Clock() { }

	void Clock::Start()
	{
		QueryPerformanceCounter(&m_start); // Get the current time
	}

	void Clock::Stop()
	{
		QueryPerformanceCounter(&m_stop); // Get the current time
	}

	float Clock::Elapsed()
	{
		QueryPerformanceCounter(&m_current); // Get the current time

		return static_cast<real>(m_current.QuadPart - m_start.QuadPart) / static_cast<real>(m_freq.QuadPart); // Return the elapsed time
	}

	float Clock::Difference()
	{
		return (m_stop.QuadPart - m_start.QuadPart) / (real)m_freq.QuadPart; // Return the difference between the start and stop times
	}

	long long Clock::Current()
	{
		QueryPerformanceCounter(&m_current); // Get the current time
		return m_current.QuadPart; // Return the current time
	}
}
