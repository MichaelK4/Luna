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
		LARGE_INTEGER m_current;
	public:
		Clock();
		~Clock();

		void Start(); // Start the clock - returns the time elapsed since Stop was called
		void Stop(); // Stop the clock - returns the time elapsed since Start was called

		float Elapsed(); // Returns the time elapsed since Start was called
		float Difference();
		long long Current();

	};
}

#endif // !CLOCK_H

