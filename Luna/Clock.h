#ifndef CLOCK_H
#define CLOCK_H
//#include "Libs.h" 
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
		Clock(); // Constructor
		~Clock(); // Destructor

		/**
	     * @brief Stops the clock
		 */
		void Start(); // Start the clock
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