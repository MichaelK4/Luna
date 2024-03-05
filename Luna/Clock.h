#ifndef CLOCK_H
#define CLOCK_H

#include "Libs.h"

namespace Luna
{
	class Clock
	{
	private:
		LARGE_INTEGER m_start;
		LARGE_INTEGER m_stop;
		LARGE_INTEGER m_freq;
	public:
		Clock();
		~Clock();

		void Start(); // Start the clock - returns the time elapsed since Stop was called
		void Stop(); // Stop the clock - returns the time elapsed since Start was called

		double Elapsed(); // Returns the time elapsed since Start was called

	};
}

#endif // !CLOCK_H

