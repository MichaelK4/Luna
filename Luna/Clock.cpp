#include "Clock.h"

namespace Luna
{
	Clock::Clock()
	{
		// set the thread affinity mask to the first core for consistent timing
		SetThreadAffinityMask(GetCurrentThread(), 1);
		
		// Get the frequency of the high-performance counter
		QueryPerformanceFrequency(&m_freq);

		// Initialize the clock
		Start();
		Stop();
	}

	Clock::~Clock() 
	{ 
		// No specific cleanup needed 
	}

	void Clock::Start()
	{
		// Record the start time
		QueryPerformanceCounter(&m_start);
	}

	void Clock::Stop()
	{
		// Record the stop time
		QueryPerformanceCounter(&m_stop); // Get the current time
	}

	float Clock::Elapsed()
	{
		// Get current time
		QueryPerformanceCounter(&m_current);

		// Calculate and return the elapsed time in seconds
		return static_cast<float>(m_current.QuadPart - m_start.QuadPart) / static_cast<float>(m_freq.QuadPart); 
	}

	float Clock::Difference()
	{
		// Calculate and return the difference between the start and stop in seconds
		return (m_stop.QuadPart - m_start.QuadPart) / (float)m_freq.QuadPart; 
	}

	long long Clock::Current()
	{
		// Get the current time
		QueryPerformanceCounter(&m_current);
		return m_current.QuadPart; 
	}
}