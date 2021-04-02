#include <iostream>
#include <chrono>

/* Example of usage:
   Timer t;
   foo();
   t.stop(); */

class Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start_timepoint;
	std::chrono::time_point<std::chrono::steady_clock> end_timepoint;
public:
	Timer() {
		start_timepoint = std::chrono::steady_clock::now();
	}
	
	void stop() {
		end_timepoint = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_timepoint - start_timepoint).count();
		std::cout << "Duration: " << duration << " ms\n";
	}
};
