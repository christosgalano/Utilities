#pragma once
#include <iostream>
#include <chrono>

class Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	std::chrono::time_point<std::chrono::steady_clock> m_EndTimepoint;
public:
	Timer() {
		m_StartTimepoint = std::chrono::steady_clock::now();
	}

	~Timer() {
		Stop();
	}

	void Stop() {
		m_EndTimepoint = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTimepoint - m_StartTimepoint).count();
		std::cout << "Duration: " << duration << " ms\n";
	}
};