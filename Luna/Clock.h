#ifndef CLOCK_H
#define CLOCK_H

/* Windows is a library that provides
functions for working with the
Windows operating system. */
#include <Windows.h>

namespace Luna 
{
	/**
	 * @class Clock
	 * @brief A high-precision timer class for measuring elapsed time
	 */
	class Clock 
	{
	private:
		LARGE_INTEGER m_start; ///< Start time of the clock
		LARGE_INTEGER m_stop; ///< Stop time of the clock
		LARGE_INTEGER m_freq; ///< Frequency of the performance counter
		LARGE_INTEGER m_current; ///< Current time
	public:
		/**
		 * @brief Constructor for Clock class
		 * Initializes the clock and retrieves the frequency of the performance counter.
		 * Uses LARGE_INTEGER for high-precision timing operations, suitable for measuring
		 * small time intervals accurately.
		 */
		Clock(); // Constructor
		~Clock(); // Destructor

		/**
		 * @brief Start the clock
		 * Records the current time as the start time using QueryPerformanceCounter.
		 * Uses LARGE_INTEGER to ensure precision in capturing start time.
		 */
		void Start();
		
		/**
		 * @brief Stop the clock
		 * Records the current time as the stop time using QueryPerformanceCounter.
		 * Uses LARGE_INTEGER to ensure precision in capturing stop time.
		 */
		void Stop(); // Stop the clock

		/**
		 * @brief Calculates the time elapsed since Start() was called
		 * @return Time elapsed in seconds
		 */
		float Elapsed(); // Returns the time elapsed since Start was called
		
		/**
		 * @brief Calculates the time difference between Start() and Stop() calls
		 * @return Time difference in seconds
		 */
		float Difference();

		/**
		 * @brief Gets the current time
		 * @return Current time as a long long integer
		 */
		long long Current(); // Returns the current time
	};
}

#endif // !CLOCK_H