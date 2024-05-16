#include "Clock.h"


namespace Luna
{
	Clock::Clock()
	{
		SetThreadAffinityMask(GetCurrentThread(), 1);
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

	float Clock::Elapsed()
	{
		QueryPerformanceCounter(&m_current);

		return static_cast<real>(m_current.QuadPart - m_start.QuadPart) / static_cast<real>(m_freq.QuadPart);
	}

	float Clock::Difference()
	{
		return (m_stop.QuadPart - m_start.QuadPart) / (real)m_freq.QuadPart;
	}

	long long Clock::Current()
	{
		QueryPerformanceCounter(&m_current);
		return m_current.QuadPart;
	}
}
