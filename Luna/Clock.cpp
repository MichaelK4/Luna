#include "Clock.h"


namespace Luna
{
	Clock::Clock()
	{
		QueryPerformanceFrequency(&m_freq);

		Start();
		Stop();
	}

	Clock::~Clock() { }

	void Clock::Start()
	{
		QueryPerformanceCounter(&m_start);
	}

	void Clock::Stop()
	{
		QueryPerformanceCounter(&m_stop);
	}

	double Clock::Elapsed()
	{
		LARGE_INTEGER cur;
		QueryPerformanceCounter(&cur);

		return static_cast<double>(cur.QuadPart - m_start.QuadPart) / static_cast<double>(m_freq.QuadPart);
	}
}
