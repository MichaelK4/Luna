#ifndef CLOCK_H
#define CLOCK_H
#include "Libs.h"

namespace Luna 
{
	class Clock 
	{
	private:
		LARGE_INTEGER m_start; // Start time
		LARGE_INTEGER m_stop; // Stop time
		LARGE_INTEGER m_freq; // Frequency
		LARGE_INTEGER m_current; // Current time
	public:
		Clock(); // Constructor
		~Clock(); // Destructor

		void Start(); // Start the clock - returns the time elapsed since Stop was called
		void Stop(); // Stop the clock - returns the time elapsed since Start was called

		float Elapsed(); // Returns the time elapsed since Start was called
		float Difference(); // Returns the time elapsed since Stop was called
		long long Current(); // Returns the current time
	};
}

#endif // !CLOCK_H

